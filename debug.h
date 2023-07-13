#include "rovdatatypes.h"
#include <Arduino.h>
#include <cstdint>

class Debug {
  public:
    union debugFlags {
        int8_t flags = 0;
        struct {
            bool depthSensor   : 1;
            bool analogSensors : 1;
            bool imu           : 1;
            bool thrusters     : 1;
            bool manipulator   : 1;
            bool cameras       : 1;
            bool regulators    : 1;
            bool catgirl       : 1;
        };
    };
    Debug() = delete;
    Debug(RovTelemetry *tele, RovControl *ctrl, RovAuxControl *auxCtrl)
        : tele(tele), ctrl(ctrl), auxCtrl(auxCtrl){};
    void setDebugFlags(debugFlags flags) { this->flags = flags; };
    void debugHandler();
    void menu();

    const __FlashStringHelper * ayana =F(
"  oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo++oooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo:. +ooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo:..~..ooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooo+:++oooooooooooooooooooooooooo:...+~ .:oooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooo+. .:~~~~~:~::+++++:::~::::::....~o+~.~oooooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooo+..~++:...        .   .      .~.+oo+.~oooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooo:. ~oo+:.~.      ..... ..    ..+oo+..oooooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooo~. +ooo:.. ........... ......:o+o: ~oooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooo~ +oo+:.....~..~...~.~~.~~..:+oo~ :oooooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooooo:.:+~...~~~.~~..~~~~..~~~.. ~++...oooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooo+~~~ .....~.:~~~~~...~... . ...  +ooooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooooooo+~..  . .....~~.~.. ........  ..~ooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooo+~~:.... .....~.....~:+++:~..   ..ooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooo:.:~.~.~.....~~~~~.~+~.~......    +oooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooooooooooooooooooooo...~..~+~~~:~:~~~~~oo+  ~.~....   +oooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooooo+ ......~~.~~:~~~:~:++++.~~~...    .oooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooooo~  ...~:+. ~+++++++.   ..~~.. .     +ooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooo+:..  ...+o+++ooooooo+:::+~~~. ..     :ooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooooo:...   ..~++++o+ooo+o+++++~~~. .   .   ooooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooooo+~.....~. ..+ooooo+oooo+o+++~~. .    .  ::oooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooooo++++++++oo+:..~++ooo+++ooooo+:~..     ... o:+ooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooooooo+:++oo+oooo++o+.~::++++oo++:~~:..   . . ..:.+o~ooooooooooooooooo\n\r\
  oooooooooooooooooooooooooooooooo++.~:oo+o+++++++++:~..           .~~. ........:+~o+:oooooooooooooooo\n\r\
  ooooooooooooooooooooooooooo+++:..~:+ooo+++++++++++~. .   ~+~.~::: ~+:::~.  ...~o:+o~:ooooooooooooooo\n\r\
  ooooooooooooooooooooooo+::.....:++oooo++++++ooo+o+~~.. .~++++++~  ++ooo+. .....+++oo :oooooooooooooo\n\r\
  ooooooooooooooooooo+:.     . .+oooo++++:++o++oo+:. ..~:.:++:~....:o:+++o+... ..:o:oo+ :ooooooooooooo\n\r\
  ooooooooooooooo+:.  .~:+~..:++ooooooo++++o++::~.~.~:+~.~~~:..~...~++++++o+.. . .++:oo+ ~oooooooooooo\n\r\
  oooooooooooo+:  .:++oo+~:+oo+++oo++o+o++::...~++o+~++:~:~~~~~:~.:+++o++oo+~. ~: ~o~+oo+ .+oooooooooo\n\r\
  o+oooooooo+:  .+oooo+~~ooo++++++o+++++~ . .:+oooo+:+:~::++++:.~+o++++++o++....o+.~+.ooo+::+ooooooooo\n\r\
  o++oooooo+.  ~+oooo+.+o+::::~~~:~~:+o~ .~~o+o++o++:::+~:+:~~.~ooo++:+o++++: . +oo:~ :ooo+o+++ooooooo\n\r\
  oo+:+ooo+. .~+oo++:..~.~~~..     .~:++.+:o+++++o+++:++::~...~oo+oo++++++++~ . ~oooo..+ooooo+++++oooo\n\r\
  oooo+~::...:~+~.       .~.  .....    ~:o++++++++++:+++++....+ooo++:+o+++o:  .. +ooo~ :::+ooooo+~~++o\n\r\
  oooooo+~..+:~    ..          ...      .~~:+++++++++++++::. ~++++++:++oo++ ..  ..:oo: :++~.~++o::o+.~\n\r\
  ooooooo+~~o:~ .......     ...~...        ~:::+++++++++:+:  .+++++:+++++++~....~. .+~ ~ooo+~ .~.+oo+.\n\r\
  oooooooo++o:. .........   ..~....     ..  .++++++++++++++. ~~++::+o+o++++:~...o+......:++++.  +oooo+\n\r\
  oooooooo++: ..  .... ...  .... ..      .. ..:+++++++++++++::+:+::+o++ooo+::~. ~oo:.... .:+++~.+ooooo\n\r\
  oooooooo+. .   .......   ..~~....      ......~++++++++++++++++:+++o++o+o+:::.  +oo~ .....~:o+:oooooo\n\r\
  ooooooo+  .  .:  ... ... .....~.         . .. .:+++oo+++++:::::oo+oooo++:::::~ .+o. ...~.  ..+oooooo\n\r\
  oooooo+~ . .+oo~ ....... ......       . ..~...  ~ooo+o++++.~+:+oo++++:~~~~::~+. .:   +: ~. . .~+oooo\n\r\
  oooooo+~  ~oooo+ ......  .......        ........ .++o+o+++~.::+oo+~:~:~.~:~~~:+. .   oo+... . +~:+oo\n\r\
  ooooooo. :oooooo: .. ..  ~.~... .       ........~  ~++++o+~ :+:oo+::~:~~~:~~~~+:   .:~+o+~. . :o++++\n\r\
  oooo+oo::ooooooo+   .    ..... .         ... . ...  .+++++~ ~+++oo:~~~~.+~~~~.:+ ...~.~~:~.....ooooo\n\r\
  ooo+:ooooooooo+..~  ..   .....          .......~...   :+o++:++++oo:~~~~ +~~~~~:~   .. ::::~~.. +oooo\n\r\
  oooo+:++++++~~~++o+      ...              ...~.... .  ~.+oo++++:oo+~~~..+~.~~. .. ...  ~:+~:.. +oooo\n\r\
  oooooo++++++ooooooo.    ....              . ..... ..  .~ :+o++:+oo+~~~..o.~~...~... .. ~:.~:~. ooooo\n\r\
  oooooooooooooooooooo.    .                  .......    ~. .+o+++ooo..~ :o.....::~...~. ~o+~~~ :ooooo\n\r\
  oooooooooooooooooooo+                      ....... .  .~.~ .+++++oo~...o+ .~.:o+:~.~~   o++~..oooooo\n\r\
  ooooooooooooooooooooo~                       ...  ... ~~~.  ~o++ooo~..:o~...~+o+:~~~.   o++:. oooooo\n\r\
  ooooooooooooooooooooo+                            .   .~. .  :+ooo+...o:.. ~+o+:++:~~  :o:+: ~+ooooo\n\r\
  ooooooooooooooooooooo:                                 ...~ .+oooo+..+:~~.~oo++++::+~  ++++::~+ooooo\n\r\
  oooooooooooooooooooo+                                  :~..:oooooo~.+~+:~+oooooooo.~  +oooo++:+ooooo\n\r\
  ooooooooooooooooooo~.         ..                       .. .oooooo+~oo+oooooooooo+.   +oooooooooooooo\n\r\
  oooooooooooooooooo~             :~::++:~::~.  ..      .~   .oooo+:oooooooooooo+~    +ooooooooooooooo\n\r\
  ooooooooooooooooo~              ~oooooooooo++++++++:++oo.   .+oooooooooooooo+~    ~ooooooooooooooooo\n\r\
  oooooooooooooooo:     ~          +ooooooooooooooooooooooo.    ~+ooooooo++~..    ~+oooooooooooooooooo\n\r\
  ooooooooooooooo+ .   ..          .oooooooooooooooooooooooo+.     ....        ~+ooooooooooooooooooooo\n\r\
  oooooooooooooo+. .                +ooooooooooooooooooooooooo+~.         .~:+oooooooooooooooooooooooo\n\r\
  oooooooooooo+~ ... .   . .        ooooooooooooooooooooooooooooo++++++++ooooooooooooooooooooooooooooo\n\r\
  ooooooooooo+..... .   ~o+        ~oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooo~ .... ..  +oo.   ..   oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooo+~       ~+ooo. ..     +ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooooo+::::++ooooo~. .     ~ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooo~ ..     ~ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooo+.      ~oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r\
  ooooooooooooooooooooooooooo+~.~~+ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n\r"
);


  private:
    RovTelemetry  *tele;
    RovControl    *ctrl;
    RovAuxControl *auxCtrl;
    debugFlags     flags;
};
