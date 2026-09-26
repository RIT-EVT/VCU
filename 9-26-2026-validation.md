# Validation – 9/26/2026

**Tester:** Travis

**Branch / Commit:** `tmb5932/tcs-testing` @ `95ed16a`

## Summary

| # | Test                             | Result |
|---|----------------------------------|--------|
| 1 | HardMon both CAN TX line Quiet   | Pass   |
| 2 | MCUC both CAN TX line Sending    | Pass   |
| 3 | Combined CAN H/L Output is Clean | Pass   |
| 4 | Local Bench CAN-A w/ just LVSS   | Pass   |
| 5 | On bike VCU+LVSS CAN-A test      | N/A    |

---

## 1. HardMon CAN TX line

**Goal:** Confirm HardMon is not its CAN TX line.

**Expected:** CAN TX line should be held high 

**Actual:** both CAN TX lines are held high

**Result:** Pass

**Notes:** Hardmon is outputting correct signals to not impact bike at
this stage.

---

## 2. MCUC CAN TX line

**Goal:** Confirm MCUC is driving its CAN TX lines.

**Setup:** Using Saleae (Logic Analyzer) with CAN decoder to ensure proper formating of TX messages.

**Expected:** TX lines are outputting legible and expected (GFDB on MC, heartbeat & ) messages

**Actual:** TX lines are clean and transmitting data as expected

**Result:** Pass

**Notes:** MCUC is outputting proper signals to the transceiver, so any issues (if any) should be between transceiver and bus.

---

## 3. Combined CAN H/L output

**Goal:** Confirm CAN H/L on the bus with both HardMon and MCUC transmitting.

**Setup:** Danny's Oscilloscope decoding the CAN high value from the TCS's output connector, and Saleae decoding the CAN TX pin. Done for both accessory and powertrain lines.

**Expected:** Saleae decoding and Oscilloscope decoding both match the expected COB-ID and message value.

**Actual:** Saleae and oscilloscope decoding was matching and showed expected! (LVSS message on accessory and GFDB request message on MC).

**Result:** Pass

**Notes:** CAN coming from TCS def works!

---

## 4. Local Bench CAN-A w/ just LVSS

**Goal:** CAN network of just LVSS and TCS can successfully talk to eachother.

**Setup:** LVSS and TCS plugged in to 12v barrel jack, CAN Accessory H/L jumped between them. (H to H and L to L)

**Expected:** LVSS receives the values that TCS sends out.

**Actual:** LVSS was not receiving any CAN messages, and was failing to send its own... But after making some changes to how 
the data is formatted on the line and pulling LVSS's EVT-Core up to most recent it works. So yeah. Whenever ignition is off,
the LVSS receives 0 on all switches (turn all off), and when ignition on, LVSS receives all 1's (turn everything on)!

**Result:** Pass

**Notes:** No idea why it wasn't working at first but now it does!

---

## 5. On bike VCU+LVSS CAN-A test

**Goal:**

**Setup:**

**Expected:**

**Actual:**

**Result:** Pass / Fail

**Notes:**
