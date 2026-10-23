#!/usr/bin/env python3
"""Bounded final artifact validation; no scientific experiment reruns."""
from pathlib import Path
import ast,hashlib,json,re,subprocess

ROOT=Path(__file__).resolve().parents[1]
BASE='2f4648c379c474ed124e3ef10b62efcd4a783c66'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
entries=[]
gitroot=subprocess.check_output(['git','rev-parse','--show-toplevel'],cwd=ROOT,text=True).strip()
raw=subprocess.check_output(['git','ls-tree','-r',BASE,'--','research/sav/'],cwd=gitroot,text=True)
for line in raw.splitlines():
    meta,path=line.split('\t',1);mode,kind,oid=meta.split()
    entries.append(dict(path=path,mode=mode,type=kind,git_blob=oid))
assert len(entries)>90 and any(x['path']=='research/sav/goals/GLR-GOAL-004.md' for x in entries)
manifest=dict(input_commit=BASE,input_tree=subprocess.check_output(
    ['git','rev-parse',BASE+'^{tree}'],cwd=ROOT,text=True).strip(),research_entries=entries)
(ROOT/'verification/goal004_input_tree.json').write_text(json.dumps(manifest,indent=2)+'\n')
files=sorted(set(ROOT.glob('physics/goal004*'))|set(ROOT.glob('verification/goal004*'))|
             set(ROOT.glob('verification/AUDIT_GOAL004*'))|
             {ROOT/'goals/GLR-GOAL-004_REPORT.md',ROOT/'verification/GLR_GOAL004_CHECKS.md'})
files=[p for p in files if p.is_file() and p.name not in
       ('goal004_validation.json','goal004_publication.json')]
for p in files:
    if p.suffix=='.py':ast.parse(p.read_text(),filename=str(p.relative_to(ROOT)))
    if p.suffix=='.json':json.loads(p.read_text())
    if p.suffix in ('.md','.py','.json','.txt'):
        text=p.read_text()
        assert not re.search(r'/(?:Users|private/var)/[A-Za-z0-9]|(?:ghp|github_pat)_[A-Za-z0-9]{20,}',text),p
mds=[ROOT/n for n in ('README.md','HANDOFF.md','STATUS.md','SOURCES.md',
       'goals/GLR-GOAL-004.md','handoffs/MICHAEL_START_HERE.md','verification/INDEX.md',
       'verification/negative_results.md')]+[p for p in files if p.suffix=='.md']
links=[]
for p in mds:
    for target in re.findall(r'\]\(([^\n)]*\.(?:md|py|json|txt|csv|npz|png))(?:#[^)]*)?\)',p.read_text()):
        assert (p.parent/target).exists(),(p,target)
        links.append(dict(file=str(p.relative_to(ROOT)),target=target))
audit=json.loads((ROOT/'verification/goal004_audit_dynamics_results.json').read_text())
for name,digest in audit['source_sha256'].items():
    assert sha(ROOT/name)==digest,('audited artifact changed',name)
# Source run intentionally predates post-only additions; current hash is
# pinned in diagnostics, with the distinction explicitly retained.
sourcepost=json.loads((ROOT/'verification/goal004_source_geometry_diagnostics.json').read_text())
assert sha(ROOT/'verification/goal004_source_geometry.py')==sourcepost['final_script_sha256']
for result,source,key in (
    ('goal004_model_results.json','goal004_model.py','sha256'),
    ('goal004_two_mode_results.json','goal004_two_mode.py','source_sha256'),
    ('goal004_data_checks_results.json','goal004_data_checks.py','script_sha256')):
    d=json.loads((ROOT/'verification'/result).read_text())
    expected=d[key][source] if isinstance(d[key],dict) else d[key]
    assert sha(ROOT/'verification'/source)==expected,(source,result)
checks=dict(syntax_json_hashes='PASS',markdown_file_links='PASS',
    sensitive_host_strings='PASS',audited_artifacts_unchanged='PASS',
    file_count=len(files),checked_local_links=len(links),
    evidence_sha256={str(p.relative_to(ROOT)):sha(p) for p in files},
    source_run_vs_post_hash='Different versions explicitly declared; current post hash verified',
    scope='Artifact consistency only; no physics acceptance or scientific reruns')
(ROOT/'verification/goal004_validation.json').write_text(json.dumps(checks,indent=2)+'\n')
print(json.dumps({k:v for k,v in checks.items() if k!='evidence_sha256'},indent=2))
