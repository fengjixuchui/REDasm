#include "disassemblertest.h"
//#include <redasm/disassembler/disassembler.h>
//#include <redasm/plugins/assembler/assembler.h>
//#include <redasm/support/utils.h>
//#include <redasm/context.h>
#include <QStandardPaths>
#include <QApplication>
#include <iostream>
#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QDir>

#define TEST_PREFIX                      "/home/davide/Programmazione/Campioni/" // NOTE: Yes, hardcoded for now :(
#define TEST_PATH(s)                     TEST_PREFIX + String(s)

#define REPEAT_COUNT                     20
#define REPEATED(s)                      String::repeated(s, REPEAT_COUNT).c_str()

#define RED_STRING(s)                    ("\x1b[31m" + String(s) + "\x1b[0m").c_str()
#define GREEN_STRING(s)                  ("\x1b[32m" + String(s) + "\x1b[0m").c_str()
#define TEST_OK                          GREEN_STRING("OK")
#define TEST_FAIL                        RED_STRING("FAIL")

#define TEST(s, cond)                    cout << "->> " << s << "..." << ((cond) ? TEST_OK : TEST_FAIL) << endl
#define TITLE(t)                         cout << REPEATED('-') << t << " " << REPEATED('-') << endl
#define TEST_TITLE(t)                    TITLE("Testing " << t)

#define TEST_NAME(sym, s)                (sym->name == s)
#define TEST_SYMBOL(s, sym, exp)         TEST(s, (sym && exp))
#define TEST_SYMBOL_NAME(s, sym, exp, n) TEST_SYMBOL(s, sym, TEST_NAME(sym, n) && exp)

#define ADD_TEST(t, cb)                  m_tests[t] = std::bind(&DisassemblerTest::cb, this)
#define ADD_TEST_NULL(t, cb)             m_tests[t] = nullptr;
#define ADD_TEST_PATH(t, cb)             m_tests[TEST_PATH(t)] = std::bind(&DisassemblerTest::cb, this)
#define ADD_TEST_PATH_NULL(t, cb)        m_tests[TEST_PATH(t)] = nullptr;

using namespace std;

DisassemblerTest::DisassemblerTest()
{
    //ADD_TEST_PATH("PE Test/VB5CRKME.EXE", testVB5CrackMe);
    //ADD_TEST_PATH("PE Test/OllyDump.dll", testOllyDump);
    //ADD_TEST_PATH("PE Test/tn_11.exe", testTn11);
    //ADD_TEST_PATH("PE Test/HelloWorldMFC.exe", testHelloWorldMFC);
    //ADD_TEST_PATH("PE Test/TestRTTI.exe", testTestRTTI);
    //ADD_TEST_PATH("IOLI-crackme/bin-pocketPC/crackme0x01.arm.exe", testIoliARM);
    //ADD_TEST_PATH("ELF Test/helloworld32_stripped", testHw32Stripped);
    //ADD_TEST_PATH("ELF Test/jmptable", testJmpTable);
    //ADD_TEST_PATH("ELF Test/pwrctl_be", testPwrCtlBE);

    //ADD_TEST_PATH_NULL("PE Test/CorruptedIT.exe", nullptr);

    //ContextSettings ctxsettings;
    //ctxsettings.tempPath = qUtf8Printable(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    //ctxsettings.runtimePath = qUtf8Printable(QDir::currentPath());
    //ctxsettings.logCallback =[](const String&) { };
    //ctxsettings.ignoreproblems = true;
    //r_ctx->init(ctxsettings);
}

DisassemblerTest::~DisassemblerTest() { }

void DisassemblerTest::runTests()
{
    /*
    for(const TestItem& test : m_tests)
    {
        r_pm->unloadAll();
        QString testpath; // = Convert::to_qstring(test.first);
        QFileInfo fi(testpath);

        if(!fi.exists())
        {
            cout << "!!! SKIPPING TEST '" << qUtf8Printable(fi.fileName()) << "', file not found..." << endl << endl;
            return;
        }

        TEST_TITLE(qUtf8Printable(fi.fileName()));
        m_buffer = MemoryBuffer::fromFile(qUtf8Printable(testpath));

        if(m_buffer->empty())
        {
            cout << "!!! File is empty" << endl << endl;
            return;
        }

        this->runCurrentTest(test.first, test.second);
        cout << REPEATED('-') << REPEATED('-') << REPEATED('-') << endl << endl;
    }
    */
}

string DisassemblerTest::replaceAll(std::string str, const std::string &from, const std::string &to)
{
    if(from.empty())
        return str;

    size_t idx = 0;

    while((idx = str.find(from, idx)) != std::string::npos)
    {
        str.replace(idx, from.length(), to);
        idx += to.length();
    }

    return str;
}

//void DisassemblerTest::runCurrentTest(const String& filepath, const TestCallback &cb)
//{
    // LoadRequest request(filepath, m_buffer);
    // REDasm::PluginList loaders = r_pm->getLoaders(request); //, true);
    // TEST("Loader", !loaders.empty());

    // if(loaders.empty())
    //     return;

    // const PluginInstance *assemblerpi = nullptr, *loaderpi = loaders.first();
    // REDasm::Loader* loader = plugin_cast<REDasm::Loader>(loaderpi);
    // loader->init(request);

    // assemblerpi = r_pm->findAssembler(loader->assembler().id);
    // TEST("Assembler", assemblerpi);

    // if(!assemblerpi)
    //     return;

    // REDasm::Assembler* assembler = plugin_cast<REDasm::Assembler>(assemblerpi);
    // assembler->init(loader->assembler());

    // m_disassembler = std::make_unique<Disassembler>(assembler, loader);
    // m_document = m_disassembler->document();

    // cout << "->> Disassembler...";
    // m_disassembler->disassemble();
    // cout << TEST_OK << endl;

    // if(cb)
    //     cb();
//}

//void DisassemblerTest::testTrampolines(const std::map<address_t, String> &trampolines)
//{
    //for(auto& trampoline : trampolines)
    //{
        //const Symbol* symbol = m_document->symbol(trampoline.first);
        //TEST_SYMBOL_NAME(("Trampoline " + trampoline.second + " @  " + String::hex(trampoline.first)).c_str(), symbol, symbol->isFunction(), trampoline.second);
    //}
//}

//void DisassemblerTest::testVBEvents(const std::map<address_t, String> &vbevents)
//{
    //for(auto& vbevent : vbevents)
    //{
        //String procname = vbevent.second;
        //procname.replace("::", "_");

        //const Symbol* symbol = m_document->symbol(vbevent.first);
        //TEST_SYMBOL_NAME(("Event " + vbevent.second + " @ " + String::hex(vbevent.first)).c_str(), symbol, symbol->isFunction(), procname);
    //}
//}

/*
void DisassemblerTest::testOllyDump()
{
    // const Symbol* symbol = m_document->symbol(0x00403BDC);
    // TEST_SYMBOL("Checking Function @ 00403bdc", symbol, symbol->isFunction());

    // CachedInstruction instruction = m_document->instruction(0x00403BEA);
    // TEST("Checking CALL @ 0x00403BEA", instruction);

    // if(!instruction)
    //     return;

    // TEST("Validating CALL @ 0x00403BEA target", instruction->typeIs(Instruction::Call) && m_disassembler->getTargetsCount(instruction->address));

    // symbol = m_document->symbol(0x00407730);
    // TEST_SYMBOL("Checking if target is pointer", symbol, symbol->is(Symbol::Pointer));

    // if(!symbol)
    //     return;

    // symbol = m_disassembler->dereferenceSymbol(symbol);
    // TEST_SYMBOL("Checking if dereferenced pointer is a function", symbol, symbol->isFunction());
}

void DisassemblerTest::testVB5CrackMe()
{
    // const Symbol* symbol = m_document->symbol(0x0040110E);
    // TEST_SYMBOL_NAME("Import VB5 ThunRTMain", symbol, symbol->is(Symbol::Function), "_msvbvm50.dll_ThunRTMain");

    // std::map<address_t, String> trampolines;
    // trampolines[0x004010C0] = "_msvbvm50.dll___vbaExitProc";
    // trampolines[0x004010C6] = "_msvbvm50.dll___vbaFreeVarList";
    // trampolines[0x004010CC] = "_msvbvm50.dll___vbaVarDup";
    // trampolines[0x004010D2] = "_msvbvm50.dll_rtcMsgBox";
    // trampolines[0x004010D8] = "_msvbvm50.dll___vbaFreeObj";
    // trampolines[0x004010DE] = "_msvbvm50.dll___vbaFreeStr";
    // trampolines[0x004010E4] = "_msvbvm50.dll___vbaHresultCheckObj";
    // trampolines[0x004010EA] = "_msvbvm50.dll___vbaObjSet";
    // trampolines[0x004010F0] = "_msvbvm50.dll___vbaR8Str";
    // trampolines[0x004010F6] = "_msvbvm50.dll___vbaOnError";

    // this->testTrampolines(trampolines);

    // symbol = m_document->symbol(0x00401EB8);
    // TEST_SYMBOL("Wide String @ 0x00401EB8", symbol, symbol->is(Symbol::WideString));

    // symbol = m_document->symbol(0x00401EF8);
    // TEST_SYMBOL("Wide String @ 0x00401EF8", symbol, symbol->is(Symbol::WideString));

    // symbol = m_document->symbol(0x00401F08);
    // TEST_SYMBOL("Wide String @ 0x00401F08", symbol, symbol->is(Symbol::WideString));

    // symbol = m_document->symbol(0x00401F44);
    // TEST_SYMBOL("Wide String @ 0x00401F44", symbol, symbol->is(Symbol::WideString));

    // std::map<address_t, String> vbevents;
    // vbevents[0x004020C4] = "Form1::Command1::Click";

    // this->testVBEvents(vbevents);
}

void DisassemblerTest::testIoliARM()
{
    // CachedInstruction instruction = m_document->instruction(0x00011064);
    // TEST("Checking LDR @ 0x00011064", instruction);

    // if(!instruction)
    //     return;

    // TEST("Checking LDR's operands count", (instruction->mnemonic() == "ldr") && (instruction->operandscount >= 2));

    // Operand* op = instruction->op(1);
    // TEST("Checking LDR's operand 2", REDasm::typeIs(op, Operand::Memory));

    // const Symbol* symbol = m_document->symbol(op->u_value);
    // TEST_SYMBOL("Checking LDR's operand 2 symbol", symbol, symbol->is(Symbol::Data) && symbol->is(Symbol::Pointer));

    // symbol = m_disassembler->dereferenceSymbol(symbol);
    // TEST_SYMBOL("Checking LDR's operand 2 dereferenced string", symbol, symbol->is(Symbol::String));

    // instruction = m_document->instruction(0x00011088);
    // TEST("Checking LDR @ 0x00011088", instruction);

    // if(!instruction)
    //     return;

    // TEST("Checking LDR's operands count", (instruction->mnemonic() == "ldr") && (instruction->operandscount >= 2));

    // op = instruction->op(1);
    // TEST("Checking LDR's operand 2", REDasm::typeIs(op, Operand::Memory));

    // u64 value = 0;
    // symbol = m_document->symbol(op->u_value);
    // TEST_SYMBOL("Checking LDR's operand 2 symbol", symbol, symbol->is(Symbol::Data) && symbol->is(Symbol::Pointer));
    // TEST("Checking dereferenced value", m_disassembler->dereference(symbol->address, &value) && (value == 0x149a));
}

void DisassemblerTest::testTn11()
{
    // CachedInstruction instruction = m_document->instruction(0x004010C0);
    // TEST("Checking DlgProc @ 0x004010C0", instruction);

    // instruction = m_document->instruction(0x00401197);
    // TEST("Checking JUMP TABLE @ 0x00401197", instruction);

    // if(!instruction)
    //     return;

    // TEST("Checking TARGETS count @ 0x00401197", m_disassembler->getTargetsCount(instruction->address) == 5);

    // if(m_disassembler->getTargetsCount(instruction->address) != 5)
    //     return;

    // size_t i = 0;
    // SortedSet targets = m_disassembler->getTargets(instruction->address);

    // for(size_t i = 0; i < targets.size(); i++)
    // {
    //     address_t target = targets[i].toU64();
    //     const Symbol* symbol = m_document->symbol(target);
    //     TEST(("Checking CASE #" + String::number(i) + " @ " + String::hex(target)).c_str(), symbol && symbol->is(Symbol::Code) && m_document->instruction(target));
    //     i++;
    // }
}

void DisassemblerTest::testHw32Stripped()
{
    TEST("Checking segments", m_document->segmentsCount() > 0);

    const Symbol* symbol = m_document->symbol("main");
    TEST_SYMBOL("Checking main", symbol, symbol->isFunction());

    symbol = m_document->symbol("init");
    TEST_SYMBOL("Checking init", symbol, symbol->isFunction());

    symbol = m_document->symbol("fini");
    TEST_SYMBOL("Checking init", symbol, symbol->isFunction());
}

void DisassemblerTest::testJmpTable()
{
    TEST("Checking segments", m_document->segmentsCount() > 0);

    const Symbol* symbol = m_document->symbol("main");
    TEST_SYMBOL("Checking main", symbol, symbol->isFunction());
}

void DisassemblerTest::testPwrCtlBE()
{
    TEST("Checking segments", m_document->segmentsCount() > 0);

    const Symbol* symbol = m_document->symbol("main");
    TEST_SYMBOL("Checking main", symbol, symbol->isFunction());
}

void DisassemblerTest::testHelloWorldMFC()
{
    // std::list<String> rttiobjects = { "type_info::ptr_rtti_object",
    //                                   "CMyApp::ptr_rtti_object",
    //                                   "CMainWindow::ptr_rtti_object" };

    // for(const String& rttiobject : rttiobjects)
    // {
    //     const Symbol* symbol = m_document->symbol(rttiobject);
    //     TEST_SYMBOL(("Checking " + rttiobject).c_str(), symbol, symbol->is(Symbol::Pointer));
    // }
}

void DisassemblerTest::testTestRTTI()
{
    // std::list<String> rttiobjects = { "type_info::ptr_rtti_object",
    //                                   "std::exception::ptr_rtti_object",
    //                                   "std::bad_alloc::ptr_rtti_object",
    //                                   "std::bad_array_new_length::ptr_rtti_object",
    //                                   "BaseClass::ptr_rtti_object",
    //                                   "DerivedClass::ptr_rtti_object" };

    // for(const String& rttiobject : rttiobjects)
    // {
    //     const Symbol* symbol = m_document->symbol(rttiobject);
    //     TEST_SYMBOL(("Checking " + rttiobject).c_str(), symbol, symbol->is(Symbol::Pointer));
    // }
}
*/
