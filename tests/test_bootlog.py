from bootlog.analyze import time_to_ready
def test_before_after():
    assert time_to_ready('[ 5.480] camera pipeline ready')==5.48
    assert time_to_ready(open('bootlog/after.log').read())==1.15
    assert time_to_ready('no marker') is None
