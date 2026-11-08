"""Directed extension of the GOAL-015 fixed-infinity geometry to the late collar."""

import argparse
from hashlib import sha256
import importlib
from math import factorial
from pathlib import Path
import subprocess
import sys

from flint import acb, arb, arb_poly, arb_series, ctx


GOAL015 = "65a4af854a25ca02e40f31b00fe5ff789935bdc4"
SOURCE_FILES = {
    "source_geometry.py": "87f4a3fd94fd09c6c3ddefb826a2560a630684fe791c0f36b62d9ea42ba82851",
    "mean_flux.py": "3dedce3f8592c7dc478147da9eb8b152f95ae7f15124c36e4cbcf495337fe51e",
}


def pinned_geometry(checkout):
    checkout = Path(checkout).resolve()
    git = ["git", "-C", str(checkout)]
    assert subprocess.check_output([*git, "rev-parse", "HEAD"], text=True).strip() == GOAL015
    assert not subprocess.check_output([
        *git, "status", "--porcelain", "--", "research/sav/GLR-GOAL-015/source",
        "research/sav/GLR-GOAL-011/checks",
    ], text=True).strip()
    path = checkout / "research/sav/GLR-GOAL-015/source"
    for name, digest in SOURCE_FILES.items():
        assert sha256((path / name).read_bytes()).hexdigest() == digest, name
    sys.path[:0] = [str(path)]
    return importlib.import_module("source_geometry")


class HorizonExtension:
    def __init__(self, outer, geometry, refinement):
        self.outer = outer
        self.geometry = geometry
        self.k, self.omega = outer.k, outer.omega
        from horizon_series import gl_horizon, gl_regular

        # The normalized degree-60 profile extends through the first compact
        # knot 1+1/8. Rescale by the original independently validated gl_scale,
        # not by a floated sample.
        scale, _ = geometry.gl_scale(acb(self.k), acb(self.omega))
        self.regular = gl_regular(acb(self.k), acb(self.omega), scale, 60)
        fields = gl_horizon(acb(self.k), acb(self.omega), scale, 224)
        self.horizon_polys = []
        for field in fields:
            poly = arb_poly([q.real for q in field.series.coeffs()])
            derivatives = [poly]
            for _ in range(4):
                derivatives.append(derivatives[-1].derivative())
            self.horizon_polys.append((field, derivatives))
        self.min_radius = 1 + arb(1) / 112
        self.join = arb(9) / 8
        self.outer_index = outer.first_mean_cell
        self.limit = outer.edges[self.outer_index]
        assert -self.min_radius - 2 * (self.min_radius - 1).log() > arb("8.407362")

        left_n, left_p = self.state(self.join, None)
        right_n, right_p = outer._cell_state(0, self.join)
        assert left_n.overlaps(right_n) and left_p.overlaps(right_p)
        direct = self.regular_jets(self.join, 2)
        reconstructed = geometry.metric_jets(self.join, right_n, right_p, self.k, self.omega)
        assert all(a.overlaps(b) for row, ref in zip(direct, reconstructed)
                   for a, b in zip(row, ref))
        count = 512 * refinement
        step = (self.join - self.min_radius) / count
        inner_edges = [self.min_radius + i * step for i in range(count)]
        left_edges = []
        for a, b in zip(outer.edges[:self.outer_index], outer.edges[1:self.outer_index + 1]):
            left_edges.extend(a + j * (b - a) / refinement for j in range(refinement))
        self.edges = inner_edges + left_edges + [self.limit]
        self.count_inner = count
        self.refinement = refinement
        self.cells = [None] * (len(self.edges) - 1)

        rhs = outer.mean_cell_derivative[self.outer_index]
        outer_edge = outer.edges[self.outer_index + 1]
        mean = tuple(z - (outer_edge - self.limit) * r
                     for z, r in zip(outer.mean_outer[self.outer_index], rhs))
        for j in range(len(self.cells) - 1, -1, -1):
            a, b = self.edges[j:j + 2]
            radius = a.union(b)
            center = (a + b) / 2
            if j < self.count_inner:
                all_jets = self.regular_jets(radius, 4)
                center_jets = self.regular_jets(center, 2)
                jets = all_jets[:3]
                smooth = tuple(tuple(arb_series([all_jets[j + k][i] / factorial(k)
                                                 for k in range(3)], 3)
                                      for i in range(4)) for j in range(3))
                value = geometry.mean_rhs(center, self.k, self.omega, center_jets)
                jet = geometry.mean_rhs(arb_series([radius, 1], 3),
                                        self.k, self.omega, smooth)
                local_mass = geometry.local_mass(center, self.k, self.omega, center_jets)
            else:
                n, p = self.state(radius, j)
                nc, pc = self.state(center, j)
                value = geometry.mean_value(center, nc, pc, self.k, self.omega)
                jet = geometry.mean_derivatives(radius, n, p, self.k, self.omega, 2)
                local_mass = geometry.centered_mass(center, nc, pc, self.k, self.omega)
                jets = geometry.metric_jets(radius, n, p, self.k, self.omega)
                center_jets = geometry.metric_jets(center, nc, pc, self.k, self.omega)
            h = b - a
            jets = tuple(tuple(q.intersection(m + geometry.symmetric(h * der / 2))
                               for q, m, der in zip(jets[k], center_jets[k], jets[k + 1]))
                         for k in (0, 1)) + (jets[2],)
            integral = [
                h * z + geometry.symmetric(h**3 * q[2] / 12)
                for z, q in zip(value, jet)
            ]
            rhs = tuple(q[0].intersection(z + geometry.symmetric(h * q[1] / 2))
                        for z, q in zip(value, jet))
            cell_mean = tuple(z - (b - radius) * q for z, q in zip(mean, rhs))
            local_mass += geometry.symmetric(h * rhs[0] / 2)
            cell_mean = cell_mean[0].intersection(local_mass), cell_mean[1]
            flat = tuple(x for row in jets for x in row) + cell_mean + rhs
            assert all(x.is_finite() for x in flat)
            self.cells[j] = flat
            mean = tuple(z - q for z, q in zip(mean, integral))
        self.inner_mean = mean

    def regular_jets(self, radius, order):
        """Horizon-regular EF metric jet with Cauchy bounds on differentiated tails."""
        t = radius - 1
        inner = t.abs_upper()
        jets = [[] for _ in range(order + 1)]
        for field, polys in self.horizon_polys:
            # A disk about any point of |t| <= inner sits inside |t| <= edge.
            edge = inner + (field.radius - inner) / 32
            error = field.tail(edge)
            for j, row in enumerate(jets):
                radius_j = error * factorial(j) / (edge - inner)**j
                row.append(polys[j](t) + self.geometry.symmetric(radius_j))
        return tuple(tuple(row) for row in jets)

    def state(self, radius, cell):
        if cell is None or cell < self.count_inner:
            n, nr = (s.value(acb(radius - 1)).real for s in self.regular)
            return n, self.omega + (1 - 1 / radius) * nr / n
        return self.outer._cell_state((cell - self.count_inner) // self.refinement, radius)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("goal015_checkout", type=Path)
    parser.add_argument("--refinement", type=int, default=1)
    parser.add_argument("--precision", type=int, default=160)
    args = parser.parse_args()
    assert args.refinement > 0
    geom = pinned_geometry(args.goal015_checkout)
    ctx.prec, ctx.cap = args.precision, 240
    outer = geom.GeometryEnclosure()
    collar = HorizonExtension(outer, geom, args.refinement)
    print(f"collar cells={len(collar.cells)}, Arb precision={ctx.prec}")
    print("certified radial collar:", collar.min_radius, collar.limit)
    print("fixed-infinity mean (mu,beta) at inner endpoint:", *collar.inner_mean)
    for j, name in enumerate(("GL jet 0", "GL jet 1", "GL jet 2", "mean and first jet")):
        a, b = 4 * j, 4 * (j + 1)
        maximum = [max(cell[k].abs_upper() for cell in collar.cells) for k in range(a, b)]
        width = [max((2 * cell[k].rad()).upper() for cell in collar.cells) for k in range(a, b)]
        print(name, "max absolute upper:", *(x.str(12, radius=True) for x in maximum))
        print(name, "max full-cell diameter:", *(x.str(12, radius=True) for x in width))


if __name__ == "__main__":
    main()
