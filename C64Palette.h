#ifndef C64PALETTE_H
#define C64PALETTE_H

#include <QColor>
#include <QList>
/*
0	       black
 1	       white
 2	       red
 3	       cyan
 4 	       purple
 5	       green
 6	       blue
 7	       yellow
 8	       orange
 9	       brown
 A	 light red
 B	  dark grey
 C	       grey
 D	 light green
 E	 light blue
 F	 light grey
*/

class C64Palette {
public:
    static QColor color(int i);
};

#endif // C64PALETTE_H
