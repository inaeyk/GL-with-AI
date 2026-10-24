# GOAL-005 publication record

Starting input: 0dd5be7818b2e71d28ebff2e122ac0a318a220f5.
Authorized target: origin refs/heads/research/sav-radiation.
Sole writer: Michael Yan's primary task; two reviewers do not commit/push.
Scientific classification: MEANINGFUL_PARTIAL.

This initial record precedes the science commit. The verified science
commit and final publication checks will be appended in a provenance
closure commit. A commit cannot contain its own identifier; the closure
commit identifier is reported in the final task response and Git history.
No publication success is asserted by this pre-commit entry.

## Verified science publication

Science commit: **17048faece51b39379521c2fddff5ccb677ba40b**.
Parent / fresh-fetched pre-publication tip:
**0dd5be7818b2e71d28ebff2e122ac0a318a220f5**.

The entire outgoing set was one commit, 46 files, all research/sav.
The reviewed binary-capable Git diff was 398526 bytes, SHA256
**5f86a9ee7e58db9eb67ce7c3c7de55113edc263a0d3917b266ed6a04b9e06ac2**.
All eight historical-document modifications are annotations; original
content is preserved. No old report, data, contract text, production file,
root policy or other branch was edited. New scientific artifact and
dependency hashes are in goal005_manifest.json.

Before publication, verified branch, clean status, HEAD, complete outgoing
commit list, all paths, preserved reviewed diff hash and ancestor relation.
A fresh fetch returned the unchanged expected parent. The whitespace gate
used per-command core.whitespace=cr-at-eol to preserve captured output
bytes without changing repository/global policy. Manifest validation
passed:35 artifacts,37 dependency edges,165 historical blobs,
8 preserved annotated documents and10 local report/proof links.
Both separate AI reviewers accepted the final report's scope.

Executed normal push:
git push origin HEAD:refs/heads/research/sav-radiation
Result: 0dd5be7..17048fa, success. A subsequent fresh fetch verified
HEAD=origin/research/sav-radiation=
17048faece51b39379521c2fddff5ccb677ba40b.

Read back and compared remote/local Git blob contents:

| File under research/sav | Verified remote Git blob |
|---|---|
| goals/GLR-GOAL-005_REPORT.md | 38519a767fa8882cfc1f334134ff4a525d3e1ef0 |
| physics/goal005_enclosures.md | 55074b577e7ef6cbca574942e427d126709b3870 |
| verification/AUDIT_GOAL005_SPECTRAL.md | 895ed3727988c2c20bae89c5d3e60b7db9b31c05 |
| verification/AUDIT_GOAL005_GEOMETRY.md | 157933ca29e8f535bd194c4d2148f7b3523dc80b |
| verification/goal005_manifest.json | c80a875fd4010d62234c45f8188c45d154b18c2a |

## Provenance closure

This appended entry is the only change in the provenance closure commit;
it changes no scientific artifact or conclusion. Its parent is the verified
science commit above. The closure receives the same fresh-fetch, expected
tip, clean scope/diff, normal fast-forward and remote-readback gates.
The final task response reports the actual closure/final branch SHA after
verification; embedding its own hash here would be self-referential.
Until that readback, this paragraph is a procedure, not a success claim.

The scientific terminal classification remains **MEANINGFUL_PARTIAL**.
Full M2 and physical radiation gates remain open. No collaborator message,
production action, PR/merge, force push, manuscript action or automatic
follow-up was performed.
