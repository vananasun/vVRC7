#include <map>
#include "vananagui/VananaGUI.h"
#include "vananagui/platform/Thread.h"
#include "vananagui/Vanme.h"
#include "common.h"

static std::map<int, VananaGUI&>* s_references = new std::map<int, VananaGUI&>;

void Van::insert(VananaGUI& ref) {
    s_references->insert({Thread::GetThreadId(), ref});
}

VananaGUI& Van::me() {
    return s_references->find(Thread::GetThreadId())->second;
}
