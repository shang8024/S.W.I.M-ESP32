const int char_map_size = 95;
// 5x8 font, char 0x20-0x7E, 0x20 is space
const unsigned int charMap[][8] = {
  { 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x04, 0x00 },  //0x21,"!"
  { 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00 },  //0x22,"""
  { 0x0A, 0x0A, 0x1F, 0x0A, 0x1F, 0x0A, 0x0A, 0x00 },  //0x23,"#"
  { 0x04, 0x0F, 0x14, 0x0E, 0x05, 0x1E, 0x04, 0x00 },  //0x24,"$"
  { 0x18, 0x19, 0x02, 0x04, 0x08, 0x13, 0x03, 0x00 },  //0x25,"%"
  { 0x0C,0x12,0x14,0x08,0x15,0x12,0x0D,0x00 },  //0x26,"&"
  { 0x0C,0x04,0x08,0x00,0x00,0x00,0x00,0x00 },  //0x27,"'"
  { 0x02,0x04,0x08,0x08,0x08,0x04,0x02,
    0x00 },  //0x28,"("
  { 0x08,
    0x04,
    0x02,
    0x02,
    0x02,
    0x04,
    0x08,
    0x00 },  //0x29,")"
  { 0x00,
    0x04,
    0x15,
    0x0E,
    0x15,
    0x04,
    0x00,
    0x00 },  //0x2a,"*"
  { 0x00,
    0x04,
    0x04,
    0x1F,
    0x04,
    0x04,
    0x00,
    0x00 },  //0x2b,"+"
  { 0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x04,
    0x08,
    0x00 },  //0x2c,","
  { 0x00,
    0x00,
    0x00,
    0x1F,
    0x00,
    0x00,
    0x00,
    0x00 },  //0x2d,"-"
  { 0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0C,
    0x0C,
    0x00 },  //0x2e,"."
  { 0x00,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x00,
    0x00 },  //0x2f,"/"
  { 0x0E,
    0x11,
    0x13,
    0x15,
    0x19,
    0x11,
    0x0E,
    0x00 },  //0x30,"0"
  { 0x04,
    0x0C,
    0x04,
    0x04,
    0x04,
    0x04,
    0x0E,
    0x00 },  //0x31,"1"
  { 0x0E,
    0x11,
    0x01,
    0x02,
    0x04,
    0x08,
    0x1F,
    0x00 },  //0x32,"2"
  { 0x1F,
    0x02,
    0x04,
    0x02,
    0x01,
    0x11,
    0x0E,
    0x00 },  //0x33,"3"
  { 0x02,
    0x06,
    0x0A,
    0x12,
    0x1F,
    0x02,
    0x02,
    0x00 },  //0x34,"4"
  { 0x1F,
    0x10,
    0x1E,
    0x01,
    0x01,
    0x11,
    0x0E,
    0x00 },  //0x35,"5"
  { 0x06,
    0x08,
    0x10,
    0x1E,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x36,"6"
  {
    0x1F,
    0x01,
    0x02,
    0x04,
    0x08,
    0x08,
    0x08,
    0x00 },  //0x37,"7"
  { 0x0E,
    0x11,
    0x11,
    0x0E,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x38,"8"
  { 0x0E,
    0x11,
    0x11,
    0x0F,
    0x01,
    0x02,
    0x0C,
    0x00 },  //0x39,"9"
  { 0x00,
    0x0C,
    0x0C,
    0x00,
    0x0C,
    0x0C,
    0x00,
    0x00 },  //0x3a,":"
  { 0x00,
    0x0C,
    0x0C,
    0x00,
    0x0C,
    0x04,
    0x08,
    0x00 },  //0x3b,";"
  { 0x02,
    0x04,
    0x08,
    0x10,
    0x08,
    0x04,
    0x02,
    0x00 },  //0x3c,"<"
  { 0x00,
    0x00,
    0x1F,
    0x00,
    0x1F,
    0x00,
    0x00,
    0x00 },  //0x3d,"="
  { 0x08,
    0x04,
    0x02,
    0x01,
    0x02,
    0x04,
    0x08,
    0x00 },  //0x3e,">"
  { 0x0E,
    0x11,
    0x01,
    0x02,
    0x04,
    0x00,
    0x04,
    0x00 },  //0x3f,"?"
  { 0x0E,
    0x11,
    0x01,
    0x0D,
    0x15,
    0x15,
    0x0E,
    0x00 },  //0x40,"@"
  { 0x0E,
    0x11,
    0x11,
    0x11,
    0x1F,
    0x11,
    0x11,
    0x00 },  //0x41,"A"
  { 0x1E,
    0x11,
    0x11,
    0x1E,
    0x11,
    0x11,
    0x1F,
    0x00 },  //0x42,"B"
  { 0x0E,
    0x11,
    0x10,
    0x10,
    0x10,
    0x11,
    0x0E,
    0x00 },  //0x43,"C"
  { 0x1C,
    0x12,
    0x11,
    0x11,
    0x11,
    0x12,
    0x1C,
    0x00 },  //0x44,"D"
  { 0x1F,
    0x10,
    0x10,
    0x1E,
    0x10,
    0x10,
    0x1F,
    0x00 },  //0x45,"E"
  { 0x1F,
    0x10,
    0x10,
    0x1E,
    0x10,
    0x10,
    0x10,
    0x00 },  //0x46,"F"
  { 0x0E,
    0x11,
    0x10,
    0x17,
    0x11,
    0x11,
    0x0F,
    0x00 },  //0x47,"G"
  { 0x11,
    0x11,
    0x11,
    0x1F,
    0x11,
    0x11,
    0x11,
    0x00 },  //0x48,"H"
  { 0x0E,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x0E,
    0x00 },  //0x49,"I"
  { 0x07,
    0x02,
    0x02,
    0x02,
    0x02,
    0x12,
    0x0C,
    0x00 },  //0x4a,"J"
  { 0x11,
    0x12,
    0x14,
    0x18,
    0x14,
    0x12,
    0x11,
    0x00 },  //0x4b,"K"
  { 0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0x1F,
    0x00 },  //0x4c,"L"
  { 0x11,
    0x1B,
    0x15,
    0x15,
    0x11,
    0x11,
    0x11,
    0x00 },  //0x4d,"M"
  { 0x11,
    0x11,
    0x19,
    0x15,
    0x13,
    0x11,
    0x11,
    0x00 },  //0x4e,"N"
  { 0x0E,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x4f,"O"
  { 0x1E,
    0x11,
    0x11,
    0x1E,
    0x10,
    0x10,
    0x10,
    0x00 },  //0x50,"P"
  { 0x0E,
    0x11,
    0x11,
    0x11,
    0x15,
    0x12,
    0x0D,
    0x00 },  //0x51,"Q"
  { 0x1E,
    0x11,
    0x11,
    0x1E,
    0x14,
    0x12,
    0x11,
    0x00 },  //0x52,"R"
  { 0x0F,
    0x10,
    0x10,
    0x0E,
    0x01,
    0x01,
    0x1E,
    0x00 },  //0x53,"S"
  { 0x1F,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x00 },  //0x54,"T"
  { 0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x55,"U"
  { 0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x0A,
    0x04,
    0x00 },  //0x56,"V"
  { 0x11,
    0x11,
    0x11,
    0x15,
    0x15,
    0x15,
    0x0A,
    0x00 },  //0x57,"W"
  { 0x11,
    0x11,
    0x0A,
    0x04,
    0x0A,
    0x11,
    0x11,
    0x00 },  //0x58,"X"
  { 0x11,
    0x11,
    0x11,
    0x0A,
    0x04,
    0x04,
    0x04,
    0x00 },  //0x59,"Y"
  { 0x1F,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x1F,
    0x00 },  //0x5a,"Z"
  { 0x1C,
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0x1C,
    0x00 },  //0x5b,"["
  { 0x00,
    0x10,
    0x08,
    0x04,
    0x02,
    0x01,
    0x00,
    0x00 },  //0x5c,"\"
  { 0x0E,
    0x02,
    0x02,
    0x02,
    0x02,
    0x02,
    0x0E,
    0x00 },  //0x5d,"]"
  { 0x04,
    0x0A,
    0x11,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00 },  //0x5e,"^"
  { 0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0x00 },  //0x5f,"_"
  { 0x08,
    0x04,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00 },  //0x60,"`"
  { 0x00,
    0x00,
    0x0E,
    0x01,
    0x0F,
    0x11,
    0x0F,
    0x00 },  //0x61,"a"
  { 0x10,
    0x10,
    0x16,
    0x19,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x62,"b"
  { 0x00,
    0x00,
    0x0E,
    0x10,
    0x10,
    0x11,
    0x0E,
    0x00 },  //0x63,"c"
  { 0x01,
    0x01,
    0x0D,
    0x13,
    0x11,
    0x11,
    0x0F,
    0x00 },  //0x64,"d"
  { 0x00,
    0x00,
    0x0E,
    0x11,
    0x1F,
    0x10,
    0x0E,
    0x00 },  //0x65,"e"
  { 0x06,
    0x09,
    0x08,
    0x1C,
    0x08,
    0x08,
    0x08,
    0x00 },  //0x66,"f"
  { 0x00,
    0x0F,
    0x11,
    0x11,
    0x0F,
    0x01,
    0x0E,
    0x00 },  //0x67,"g"
  { 0x10,
    0x10,
    0x16,
    0x19,
    0x11,
    0x11,
    0x11,
    0x00 },  //0x68,"h"
  { 0x04,
    0x00,
    0x0C,
    0x04,
    0x04,
    0x04,
    0x0E,
    0x00 },  //0x69,"i"
  { 0x02,
    0x00,
    0x06,
    0x02,
    0x02,
    0x12,
    0x0C,
    0x00 },  //0x6a,"j"
  { 0x10,
    0x10,
    0x12,
    0x14,
    0x18,
    0x14,
    0x12,
    0x00 },  //0x6b,"k"
  { 0x0C,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x0E,
    0x00 },  //0x6c,"l"
  { 0x00,
    0x00,
    0x1A,
    0x15,
    0x15,
    0x11,
    0x11,
    0x00 },  //0x6d,"m"
  { 0x00,
    0x00,
    0x16,
    0x19,
    0x11,
    0x11,
    0x11,
    0x00 },  //0x6e,"n"
  { 0x00,
    0x00,
    0x0E,
    0x11,
    0x11,
    0x11,
    0x0E,
    0x00 },  //0x6f,"o"
  { 0x00,
    0x00,
    0x1E,
    0x11,
    0x1E,
    0x10,
    0x10,
    0x00 },  //0x70,"p"
  { 0x00,
    0x00,
    0x0D,
    0x13,
    0x1F,
    0x01,
    0x01,
    0x00 },  //0x71,"q"
  { 0x00,
    0x00,
    0x16,
    0x19,
    0x10,
    0x10,
    0x10,
    0x00 },  //0x72,"r"
  { 0x00,
    0x00,
    0x0E,
    0x10,
    0x0E,
    0x01,
    0x1E,
    0x00 },  //0x73,"s"
  { 0x08,
    0x08,
    0x1C,
    0x08,
    0x08,
    0x09,
    0x06,
    0x00 },  //0x74,"t"
  { 0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x13,
    0x0D,
    0x00 },  //0x75,"u"
  { 0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x0A,
    0x04,
    0x00 },  //0x76,"v"
  { 0x00,
    0x00,
    0x11,
    0x11,
    0x15,
    0x15,
    0x0A,
    0x00 },  //0x77,"w"
  { 0x00,
    0x00,
    0x11,
    0x0A,
    0x04,
    0x0A,
    0x11,
    0x00 },  //0x78,"x"
  { 0x00,
    0x00,
    0x11,
    0x11,
    0x0F,
    0x01,
    0x0E,
    0x00 },  //0x79,"y"
  { 0x00,
    0x00,
    0x1F,
    0x02,
    0x04,
    0x08,
    0x1F,
    0x00 },  //0x7a,"z"
  { 0x02,
    0x04,
    0x04,
    0x08,
    0x04,
    0x04,
    0x02,
    0x00 },  //0x7b,"{"
  { 0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x04,
    0x00 },  //0x7c,"|"
  { 0x08,
    0x04,
    0x04,
    0x02,
    0x04,
    0x04,
    0x08,
    0x00 },  //0x7d,"}"
  { 0x00,
    0x00,
    0x08,
    0x15,
    0x02,
    0x00,
    0x00,
    0x00 },  //0x7e,"~"
  { 0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0x00 }
};
