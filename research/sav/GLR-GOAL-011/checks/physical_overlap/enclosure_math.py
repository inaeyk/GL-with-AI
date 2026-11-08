"""Centered Acb enclosures for analytic source coefficients."""
from dataclasses import dataclass

from flint import acb


@dataclass(frozen=True)
class Dual:
    value: acb
    partials: tuple

    def lift(self,other):
        if isinstance(other,Dual):
            return other
        try:
            return Dual(acb(other),(acb(0),)*len(self.partials))
        except (TypeError,ValueError):
            return NotImplemented

    def __add__(self,other):
        other=self.lift(other)
        if other is NotImplemented:
            return NotImplemented
        return Dual(self.value+other.value,tuple(a+b for a,b in zip(self.partials,other.partials)))

    __radd__=__add__

    def __neg__(self):
        return Dual(-self.value,tuple(-q for q in self.partials))

    def __sub__(self,other):
        other=self.lift(other)
        return NotImplemented if other is NotImplemented else self+(-other)

    def __rsub__(self,other):
        return (-self)+other

    def __mul__(self,other):
        other=self.lift(other)
        if other is NotImplemented:
            return NotImplemented
        return Dual(self.value*other.value,tuple(a*other.value+self.value*b
                    for a,b in zip(self.partials,other.partials)))

    __rmul__=__mul__

    def __truediv__(self,other):
        other=self.lift(other)
        if other is NotImplemented:
            return NotImplemented
        ratio=self.value/other.value
        return Dual(ratio,tuple((a-ratio*b)/other.value for a,b in
                                zip(self.partials,other.partials)))

    def __rtruediv__(self,other):
        other=self.lift(other)
        return NotImplemented if other is NotImplemented else other/self

    def __pow__(self,n):
        value=self.value**n
        if not n:
            return Dual(value,(acb(0),)*len(self.partials))
        return Dual(value,tuple(n*self.value**(n-1)*a for a in self.partials))

    def sqrt(self):
        assert self.value.real>0
        root=self.value.sqrt()
        return Dual(root,tuple(a/(2*root) for a in self.partials))


def centered(function,arguments):
    """Complex mean-value inclusion on the product of convex Acb boxes."""
    points=tuple(acb(v.real.mid(),v.imag.mid()) for v in arguments)
    point_values=function(*points)
    count=len(arguments)
    differentiated=function(*(Dual(x,tuple(acb(i==j) for j in range(count)))
                               for i,x in enumerate(arguments)))
    return [center+sum(d*(x-x0) for d,x,x0 in zip(y.partials,arguments,points))
            for center,y in zip(point_values,differentiated)]
