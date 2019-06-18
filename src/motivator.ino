#define _motivatorField 2 // the number of fieldMotivators that need to be primed to make a jump
#define prev 1
#define current 0
/*
 *  Field Motivator Prime Map: list of  fieldMotivators, each as its current primed value and previous primed value
 */
int motivators[_motivatorField][2] = {
  {0, 0},
  {0, 0}
};

/*
 * Primes the motivator with the given vector;
 * Returns true if the priming is valid for a jump.
 * 
 */
bool primeMotivator(int index, int vector) {
  motivators[index][prev] = motivators[index][current];
  motivators[index][current] = vector;
  return primed(index);
}

/*
 * Sets the motivator to startup defaults
 */
void resetMotivator(int index) {
  motivators[index][current] = 0;
  motivators[index][prev] = 0;
}

void resetAllMotivators() {
  for (int motivator = 0; motivator < _motivatorField;  motivator++) {
    resetMotivator(motivator);
  }
}

/*
 * Rolls the motivator's state forward in time, such that the current and previous vectors are the same
 */
void stabilizeMotivator(int index) {
  motivators[index][prev] = motivators[index][current];
}

void stabilizeAllMotivators() {
  for (int motivator = 0; motivator < _motivatorField;  motivator++) {
    stabilizeMotivator(motivator);
  }
}

/*
 * A Motivator is only primed successfully if the newly set vector is NOT identical to the previously set vector.
 */
bool primed(int index) {
  return motivators[index][prev] != motivators[index][current];
}

/*
 * Returns true is all motivators in the field are primed.
 */
bool allPrimed() {
  for (int motivator = 0; motivator < _motivatorField; motivator++) {
    if (!primed(motivator)) {
      return false;
    }
  }
  return true;
}

/*
 * Returns true is at least 1 of the motivators in the field is primed.
 */
bool anyPrimed() {
  for (int motivator = 0; motivator < _motivatorField; motivator++) {
    if (primed(motivator)) {
      return true;
    }
  }
  return true;
}

/*
 * For debugging the motivator logic
 */
//void printMotivatorField() {
//  for (int motivator = 0; motivator < _motivatorField; motivator++) {
//    Serial.print(motivators[motivator][current]);
//    Serial.print(" - ");
//    Serial.print(motivators[motivator][prev]);
//    Serial.println();
//  }
//  Serial.println();
//}
