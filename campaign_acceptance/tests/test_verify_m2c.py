from __future__ import annotations

import unittest

from campaign_acceptance.verify_m2c import (
    Failure,
    changed_paths_from_porcelain,
    validate_changed_paths,
)


class PorcelainV1ZTests(unittest.TestCase):
    def test_first_tracked_entry_retains_its_first_path_character(self) -> None:
        self.assertEqual(
            changed_paths_from_porcelain(b" M TODO.md\0"),
            {"TODO.md"},
        )

    def test_untracked_entry(self) -> None:
        self.assertEqual(
            changed_paths_from_porcelain(b"?? run_manifests/m2c/new.json\0"),
            {"run_manifests/m2c/new.json"},
        )

    def test_multiple_entries_and_spaces(self) -> None:
        self.assertEqual(
            changed_paths_from_porcelain(
                b" M TODO.md\0?? docs/grchombo/milestone2c notes.md\0"
            ),
            {"TODO.md", "docs/grchombo/milestone2c notes.md"},
        )

    def test_rename_and_copy_include_destination_and_source(self) -> None:
        self.assertEqual(
            changed_paths_from_porcelain(
                b"R  docs/grchombo/milestone2c_new.md\0"
                b"docs/grchombo/milestone2c_old.md\0"
                b"C  run_manifests/m2c/copied.json\0"
                b"run_manifests/m2c/source.json\0"
            ),
            {
                "docs/grchombo/milestone2c_new.md",
                "docs/grchombo/milestone2c_old.md",
                "run_manifests/m2c/copied.json",
                "run_manifests/m2c/source.json",
            },
        )

    def test_rename_without_source_is_rejected(self) -> None:
        with self.assertRaisesRegex(Failure, "lacks its source path"):
            changed_paths_from_porcelain(b"R  TODO.md\0")

    def test_allowed_paths_are_accepted(self) -> None:
        validate_changed_paths(
            {
                "TODO.md",
                "code/BlackStringToy/tests/chombo_m2c_acceptance/runner.py",
                "run_manifests/m2c/preflight.json",
            }
        )

    def test_protected_and_out_of_scope_paths_are_rejected(self) -> None:
        with self.assertRaisesRegex(Failure, "protected path changed"):
            validate_changed_paths({"campaign_acceptance/verify_m2c.py"})
        with self.assertRaisesRegex(Failure, "out-of-scope path changed"):
            validate_changed_paths({"unrelated.txt"})

    def test_rename_cannot_hide_a_protected_source(self) -> None:
        changed = changed_paths_from_porcelain(
            b"R  TODO.md\0campaign_acceptance/verify_m2c.py\0"
        )
        with self.assertRaisesRegex(Failure, "protected path changed"):
            validate_changed_paths(changed)


if __name__ == "__main__":
    unittest.main()
