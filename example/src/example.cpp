#include <rodos.h>

static Application nameNotImportantHW("HelloWorld");


class HelloWorld : public StaticThread<> {

public:
    HelloWorld() : StaticThread<>("HelloWorld") { PRINTF(SCREEN_RED "Hello World!" SCREEN_RESET); }

    void init() {
        PRINTF(SCREEN_RED "This is init() for Printing Hello World" SCREEN_RESET);
    }

    void run() {
        PRINTF(SCREEN_RED "Hello World!" SCREEN_RESET);
    }
};

HelloWorld helloworld;