"""Parse dmesg/systemd-style boot log; report time to camera pipeline ready."""
import re,sys
PAT=re.compile(r"\[\s*(\d+\.\d+)\]")
MARKER="camera pipeline ready"
def time_to_ready(text):
    for line in text.splitlines():
        if MARKER in line.lower():
            m=PAT.search(line)
            if m: return float(m.group(1))
            m2=re.search(r"(\d+\.\d+)\s*s",line)
            if m2: return float(m2.group(1))
    return None
def main(path):
    t=time_to_ready(open(path).read())
    print(f"{path}: camera pipeline ready at {t:.3f}s" if t is not None else f"{path}: marker not found")
    return 0 if t is not None else 1
if __name__=="__main__": sys.exit(main(sys.argv[1] if len(sys.argv)>1 else "bootlog/before.log"))
