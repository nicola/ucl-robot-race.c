#ifndef ENCODERS_INCLUDED
#define ENCODERS_INCLUDED

void bumpersCmd();
void bumpersGet(sensors* Sensors);
int bumpersParse(char**, sensors*);
void bumpersSet(sensors*, int, int);
bool bumpersToBe(sensors*, sensors*);

#endif