import subprocess
def test_cpp_gateway():
    subprocess.run(["make", "-C", "gateway", "test"], check=True, capture_output=True, text=True)
    out = subprocess.run(["./test_gateway"], check=True, capture_output=True, text=True, cwd="gateway")
    assert "PASS" in out.stdout
def test_dbc_values():
    lines = [l.split() for l in open("gateway/signals.dbc") if l.startswith("SIGNAL")]
    assert lines[0][1] == "SteeringAngle" and int(lines[0][2]) == 256
