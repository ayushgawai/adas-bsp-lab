"""Simplified IEEE 802.1AS (gPTP) offset/delay estimator + simulation.

Formulas (two-way exchange):
  offset = ((t2-t1) - (t4-t3)) / 2
  delay  = ((t2-t1) + (t4-t3)) / 2
t1 master send Sync, t2 slave receive, t3 slave send Delay_Req, t4 master receive.
"""
import argparse, random, time

def compute_offset_delay(t1, t2, t3, t4):
    return ((t2 - t1) - (t4 - t3)) / 2, ((t2 - t1) + (t4 - t3)) / 2

def simulate(true_offset=0.005, delay=0.001, jitter=0.0002, n=20, seed=1):
    """Generate exchanges with slave clock = master clock + true_offset."""
    rnd = random.Random(seed); estimates = []
    for i in range(n):
        d1 = delay + rnd.uniform(-jitter, jitter); d2 = delay + rnd.uniform(-jitter, jitter)
        t1 = i * 0.1
        t2 = t1 + d1 + true_offset          # slave clock
        t3 = t2 + 0.004                      # slave clock, 4ms later
        t4 = (t3 - true_offset) + d2         # back on master clock
        estimates.append(compute_offset_delay(t1, t2, t3, t4)[0])
    return estimates

def timestamp_frames(offset, spacing=0.001):
    base = time.time()
    return {s: base + offset + i * spacing for i, s in enumerate(["camera", "radar", "imu"])}

if __name__ == "__main__":
    ap = argparse.ArgumentParser(); ap.add_argument("--simulate", action="store_true")
    ap.add_argument("--frames", type=int, default=20); ap.add_argument("--offset-ms", type=float, default=5.0)
    a = ap.parse_args()
    est = simulate(true_offset=a.offset_ms / 1000, n=a.frames)
    mean = sum(est) / len(est)
    print(f"samples={len(est)} estimated_offset_ms={mean*1000:.3f} true_offset_ms={a.offset_ms:.3f} skew_ms={(max(est)-min(est))*1000:.3f}")
    fr = timestamp_frames(mean)
    print("frame_timestamps:", {k: round(v, 6) for k, v in fr.items()})
    print(f"sensor_skew_ms={(max(fr.values())-min(fr.values()))*1000:.3f}")
