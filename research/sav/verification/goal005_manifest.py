"""Validate and pin the GOAL-005 review packet without rerunning physics."""
from pathlib import Path
import ast, hashlib, importlib.metadata, json, re, subprocess, sys
HERE=Path(__file__).resolve().parent
SAV=HERE.parent
START="0dd5be7818b2e71d28ebff2e122ac0a318a220f5"
def git(*args):
    return subprocess.check_output(["git",*args],cwd=SAV)
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
prefixes=["HANDOFF.md","STATUS.md","SOURCES.md","verification/INDEX.md","handoffs/MICHAEL_START_HERE.md"]
appends=["DECISIONS.md","verification/negative_results.md","goals/GLR-GOAL-005.md"]
preservation={}
for name in prefixes+appends:
    old=git("show",START+":research/sav/"+name).replace(b"\r\n",b"\n")
    new=(SAV/name).read_bytes().replace(b"\r\n",b"\n")
    assert old in new,("historical content not contiguous/preserved",name)
    assert new.endswith(old) if name in prefixes else new.startswith(old),("wrong annotation position",name)
    preservation[name]="original content preserved, newline-normalized; annotation only"
changed=git("diff","--name-status",START).decode().splitlines()
for row in changed:
    status,name=row.split("\t")
    assert name.startswith("research/sav/"),(status,name)
    assert status=="A" or (status=="M" and name in {"research/sav/"+n for n in prefixes+appends}),(status,name)
new=git("ls-files","--others","--exclude-standard","--full-name").decode().splitlines()
assert all(n.startswith("research/sav/") for n in new),new
files=sorted(set(HERE.glob("*goal005*"))|set(HERE.glob("*GOAL005*")))
files=[p for p in files if p.is_file() and p.name not in
       {"goal005_manifest.json","GOAL005_PUBLICATION.md","GLR_GOAL005_CHECKS.md"}]
files += [SAV/"physics/goal005_enclosures.md",SAV/"goals/GLR-GOAL-005_REPORT.md"]
edges=[]
for p in files:
    if p.suffix==".py": ast.parse(p.read_text(encoding="utf-8"))
    if p.suffix==".json":
        data=json.loads(p.read_text(encoding="utf-8"))
        for key in ["source_sha256","sha256","hashes"]:
            val=data.get(key,{})
            if not isinstance(val,dict): continue
            for name,digest in val.items():
                options=[HERE/name,SAV/name]
                target=next((q for q in options if q.is_file()),None)
                assert target,("missing dependency",p.name,name)
                basis="current artifact"
                if sha(target)!=digest and name in prefixes+appends:
                    original=git("show",START+":research/sav/"+name)
                    assert hashlib.sha256(original).hexdigest()==digest,("stale historical hash",p.name,name)
                    basis="historical input before preserved execution annotation"
                else:
                    assert sha(target)==digest,("stale dependency hash",p.name,name)
                edges.append([p.name,name,digest,basis])
        if "script_sha256" in data:
            script=HERE/("goal005_validate_gl.py" if p.name=="goal005_gl_results.json" else p.stem+".py")
            assert sha(script)==data["script_sha256"],("script hash",p.name)
        if "physics_note_sha256" in data:
            assert sha(SAV/"physics/goal005_enclosures.md")==data["physics_note_sha256"]
        for check in data.get("checks",[]):
            assert check.get("passed",check.get("pass",True)) is not False,(p.name,check)
    if p.suffix in {".md",".json",".txt",".py"}:
        text=p.read_text(encoding="utf-8")
        assert not re.search(r"[A-Za-z]:[\\/](?:Users|Windows)[\\/]",text),("host path",p.name)
        assert ("PRIVATE"+" KEY-----") not in text
links=[]
for name in ["goals/GLR-GOAL-005_REPORT.md","physics/goal005_enclosures.md"]:
    p=SAV/name
    for dest in re.findall(r"\]\(([^)]+)\)",p.read_text(encoding="utf-8")):
        if "://" not in dest and not dest.startswith("#"):
            target=p.parent/dest.split("#")[0]
            assert target.resolve()==(HERE/"goal005_manifest.json").resolve() or target.exists(),(name,dest)
            links.append([name,dest])
blobs=[]
for line in git("ls-tree","-r",START,"--","./").decode().splitlines():
    meta,name=line.split("\t",1)
    mode,kind,digest=meta.split()
    blobs.append(dict(path=name,git_blob=digest))
versions={n:importlib.metadata.version(n) for n in ["python-flint","numpy","scipy","sympy","mpmath"]}
out=dict(start_sha=START,branch=git("branch","--show-current").decode().strip(),
         classification="MEANINGFUL_PARTIAL",python=sys.version.split()[0],versions=versions,
         scope="research/sav only; no physical rerun in this validation",
         historical_preservation=preservation,input_git_blobs=blobs,
         artifact_sha256={str(p.relative_to(SAV)).replace("\\","/"):sha(p) for p in files},
         verified_dependency_edges=edges,verified_local_links=links)
(HERE/"goal005_manifest.json").write_text(json.dumps(out,indent=2)+"\n",encoding="utf-8")
print(json.dumps(dict(result="PASS",artifacts=len(files),dependency_edges=len(edges),
      historical_inputs=len(blobs),preserved_documents=len(preservation),local_links=len(links),versions=versions),indent=2))
