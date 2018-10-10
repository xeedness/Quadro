#ifndef GENERAL_INO
#define GENERAL_INO

void pause() {
    while (!Serial.available()) { }
    Serial.read();
}

#endif //GENERAL_INO