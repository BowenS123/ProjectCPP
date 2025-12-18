# ClankerFactory Test Plan

This document describes the manual testing performed to verify the functionality of the ClankerFactory game.

## Test Environment
- **OS**: Windows 11
- **Qt Version**: Qt 6.x

---

## 1. Unit Production Tests

### Test 1.1: Worker Production
**Objective**: Verify that Workers can be produced and appear in the game.

**Steps**:
1. Launch the game
2. Click the "Produce Worker" button
3. Observe the factory output and resource consumption

**Expected Result**: 
- Worker is added to the unit list
- Resources are deducted correctly
- Worker appears on screen with correct visual representation

**Actual Result**: ✅ PASS - Workers produce correctly

---

### Test 1.2: Scout Production
**Objective**: Verify that Scouts can be produced and appear in the game.

**Steps**:
1. Launch the game
2. Click the "Produce Scout" button
3. Observe the factory output and resource consumption

**Expected Result**:
- Scout is added to the unit list
- Resources are deducted correctly
- Scout appears on screen with correct visual representation

**Actual Result**: ✅ PASS - Scouts produce correctly

---

### Test 1.3: Defender Production
**Objective**: Verify that Defenders can be produced and appear in the game.

**Steps**:
1. Launch the game
2. Click the "Produce Defender" button
3. Observe the factory output and resource consumption

**Expected Result**:
- Defender is added to the unit list
- Resources are deducted correctly
- Defender appears on screen with correct visual representation

**Actual Result**: ✅ PASS - Defenders produce correctly

---

## 2. Resource Management Tests

### Test 2.1: Battery Production
**Objective**: Verify that Batteries can be produced to power units.

**Steps**:
1. Launch the game
2. Click the "Produce Battery" button
3. Check battery count in UI

**Expected Result**:
- Battery count increases
- Resources are consumed
- Batteries available for unit consumption

**Actual Result**: ✅ PASS - Battery production works correctly

---

### Test 2.2: Resource Gathering by Scouts
**Objective**: Verify that Scouts gather resources over time.

**Steps**:
1. Produce 2-3 Scouts
2. Observe resource counter over 30 seconds
3. Verify resource increase

**Expected Result**:
- Resources increase periodically
- Scouts perform gathering animation/behavior
- Resource counter updates in UI

**Actual Result**: ✅ PASS - Scouts gather resources as expected

---

### Test 2.3: Battery Consumption
**Objective**: Verify that units consume batteries to remain active.

**Steps**:
1. Produce several units (Workers, Scouts, Defenders)
2. Stop battery production
3. Observe battery count decreasing

**Expected Result**:
- Battery count decreases as units consume energy
- Units become inactive when batteries run out
- UI reflects battery shortage

**Actual Result**: ✅ PASS - Battery consumption works correctly

---

## 3. Combat Tests

### Test 3.1: Enemy Spawning
**Objective**: Verify that enemies spawn in waves.

**Steps**:
1. Start the game
2. Wait for first enemy wave
3. Observe enemy units appearing

**Expected Result**:
- Enemies spawn at designated intervals
- Enemy count displayed in UI
- Enemies move toward factory

**Actual Result**: ✅ PASS - Enemies spawn correctly in waves

---

### Test 3.2: Defender Combat
**Objective**: Verify that Defenders attack and eliminate enemies.

**Steps**:
1. Produce 3-5 Defenders
2. Wait for enemy wave
3. Observe combat interactions

**Expected Result**:
- Defenders engage enemies within range
- Enemy health decreases with attacks
- Enemies are eliminated when health reaches zero
- Defenders take damage from enemy counter-attacks

**Actual Result**: ✅ PASS - Combat system functions correctly

---

### Test 3.3: Factory Damage
**Objective**: Verify that factory takes damage when enemies reach it.

**Steps**:
1. Start game without producing Defenders
2. Allow enemies to reach factory
3. Monitor factory health

**Expected Result**:
- Factory health decreases when enemies attack
- UI shows factory damage
- Game ends (lose condition) when factory health reaches zero

**Actual Result**: ✅ PASS - Factory damage and health system works

---

## 4. Repair and Maintenance Tests

### Test 4.1: Worker Repair Functionality
**Objective**: Verify that Workers repair factory walls.

**Steps**:
1. Allow factory to take some damage
2. Produce Workers
3. Observe factory health over time

**Expected Result**:
- Factory health increases when Workers are active
- Repair rate is visible
- UI updates to show increased health

**Actual Result**: ✅ PASS - Workers repair factory correctly

---

### Test 4.2: Repair Priority
**Objective**: Verify Workers prioritize critical damage.

**Steps**:
1. Let factory take significant damage (below 50%)
2. Produce Workers
3. Observe repair behavior

**Expected Result**:
- Workers focus on most damaged sections
- Repair happens efficiently
- Factory stabilizes

**Actual Result**: ✅ PASS - Repair priority system works

---

## 5. UI and User Experience Tests

### Test 5.1: Button Responsiveness
**Objective**: Verify all UI buttons respond to clicks.

**Steps**:
1. Click each production button
2. Verify visual feedback
3. Confirm action execution

**Expected Result**:
- Buttons provide visual feedback on click
- Actions execute immediately
- No lag or unresponsive buttons

**Actual Result**: ✅ PASS - All buttons responsive

---

### Test 5.2: Resource Display Updates
**Objective**: Verify resource counters update in real-time.

**Steps**:
1. Observe resource displays during gameplay
2. Produce units and batteries
3. Gather resources with Scouts

**Expected Result**:
- Counters update immediately
- No visual glitches
- Values are accurate

**Actual Result**: ✅ PASS - Resource displays update correctly

---

## 6. Win/Lose Condition Tests

### Test 6.1: Lose Condition
**Objective**: Verify game ends when factory is destroyed.

**Steps**:
1. Start game without defenses
2. Allow enemies to destroy factory
3. Observe game end state

**Expected Result**:
- Game detects factory destruction
- Lose screen appears
- Option to restart available

**Actual Result**: ✅ PASS - Lose condition triggers correctly

---

### Test 6.2: Wave Survival
**Objective**: Verify progression through enemy waves.

**Steps**:
1. Build balanced force (Workers, Scouts, Defenders)
2. Survive multiple waves
3. Monitor wave difficulty increase

**Expected Result**:
- Waves increase in difficulty
- Game tracks wave number
- Victory condition triggers after final wave

**Actual Result**: ✅ PASS - Wave system works as designed

---

## 7. Object-Oriented Design Tests

### Test 7.1: Polymorphic Behavior
**Objective**: Verify different Clanker types exhibit unique behaviors.

**Steps**:
1. Produce one of each unit type
2. Observe their actions during gameplay
3. Verify each performs distinct role

**Expected Result**:
- Workers repair
- Scouts gather
- Defenders attack
- Each uses same base Clanker interface but behaves differently

**Actual Result**: ✅ PASS - Polymorphism works correctly

---

## 8. Edge Case Tests

### Test 8.1: Resource Depletion
**Objective**: Test behavior when resources reach zero.

**Steps**:
1. Deplete all resources
2. Attempt to produce units
3. Observe error handling

**Expected Result**:
- Production disabled when insufficient resources
- UI shows clear feedback
- No crashes or errors

**Actual Result**: ✅ PASS - Resource depletion handled gracefully

---

### Test 8.2: Maximum Unit Count
**Objective**: Test behavior with large number of units.

**Steps**:
1. Produce 50+ units of various types
2. Monitor performance
3. Verify game stability

**Expected Result**:
- Game handles large unit counts
- Performance remains acceptable
- No crashes

**Actual Result**: ✅ PASS - Large unit counts handled well

---

## Conclusion

All tests passed successfully. The ClankerFactory game demonstrates:
- ✅ Correct OOP implementation (inheritance, polymorphism, encapsulation)
- ✅ Stable resource management system
- ✅ Functional combat mechanics
- ✅ Responsive UI
- ✅ Clear win/lose conditions

The project meets all functional and technical requirements.

---