from timesync.timesync import compute_offset_delay
def test_offset_delay():
    # master 0-> slave offset +5ms, one-way delay 1ms: t1=0,t2=0.006,t3=0.010(slave),t4=0.006(master real: t3-offset+delay)
    off,dly=compute_offset_delay(0.0,0.006,0.010,0.006)
    assert abs(off-0.005)<1e-9 and abs(dly-0.001)<1e-9
def test_zero():
    off,dly=compute_offset_delay(0,0.001,0.001,0.002); assert off==0 and abs(dly-0.001)<1e-9
