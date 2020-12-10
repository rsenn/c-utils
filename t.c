xmlnode* configuration_descriptor = xml_element("configurationDescriptor") configuration_descriptor->attributes = xml_attributes("version", "65", 0);
xmlnode* logical_folder = xml_child_element("logicalFolder", configuration_descriptor) logical_folder->attributes = xml_attributes("name", "root", "displayName", "root", "projectFiles", "true", 0);
xmlnode* logical_fo1 = xml_child_element("logicalFolder", logical_folder) logical_fo1->attributes = xml_attributes("name", "HeaderFiles", "displayName", "Header Files", "projectFiles", "true", 0);
logical_folder = xml_child_element("logicalFolder", logical_fo1) logical_folder->attributes = xml_attributes("name", "f1", "displayName", "lib", "projectFiles", "true", 0);
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/buffer.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/interrupt.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/format.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/lcd44780.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/oscillator.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/ser.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/typedef.h");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("/home/roman/Dokumente/Sources/"
                                                                       "lc-meter/lib/delay.h");
logical_fo1 = xml_child_element("logicalFolder", logical_folder) logical_fo1->attributes = xml_attributes("name", "LinkerScript", "displayName", "Linker Files", "projectFiles", "true", 0);
logical_fo1 = xml_child_element("logicalFolder", logical_folder) logical_fo1->attributes = xml_attributes("name", "SourceFiles", "displayName", "Source Files", "projectFiles", "true", 0);
logical_folder = xml_child_element("logicalFolder", logical_fo1) logical_folder->attributes = xml_attributes("name", "f1", "displayName", "lib", "projectFiles", "true", 0);
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/buffer.c");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("/home/roman/Dokumente/Sources/"
                                                                       "lc-meter/lib/delay.c");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/lcd44780.c");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/ser.c");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/timer.c");
xml_child_element("itemPath", logical_folder)->children = xml_textnode("../../../lib/format.c");
xml_child_element("itemPath", logical_fo1)->children = xml_textnode("../../../src/print.c");
xml_child_element("itemPath", logical_fo1)->children = xml_textnode("../../../LC-meter.c");
xml_child_element("itemPath", logical_fo1)->children = xml_textnode("../../../src/measure.c");
logical_fo1 = xml_child_element("logicalFolder", logical_folder) logical_fo1->attributes = xml_attributes("name", "ExternalFiles", "displayName", "Important Files", "projectFiles", "false", 0);
xml_child_element("itemPath", logical_fo1)->children = xml_textnode("Makefile");
xmlnode* source_root_list = xml_child_element("sourceRootList", configuration_descriptor) xml_child_element("Elem", source_root_list)->children = xml_textnode("../../mplab");
xml_child_element("Elem", source_root_list)->children = xml_textnode("../../../src");
xml_child_element("Elem", source_root_list)->children = xml_textnode("../../../lib");
xml_child_element("projectmakefile", configuration_descriptor)->children = xml_textnode("Makefile");
xmlnode* confs = xml_child_element("confs", configuration_descriptor) xmlnode* conf = xml_child_element("conf", confs) conf->attributes = xml_attributes("name", "release", "type", "2", 0);
xmlnode* tools_set = xml_child_element("toolsSet", conf) xml_child_element("developmentServer", tools_set)->children = xml_textnode("localhost");
xml_child_element("targetDevice", tools_set)->children = xml_textnode("PIC18F2550");
xmlnode* target_header = xml_child_element("targetHeader", tools_set) xmlnode* target_plugin_board = xml_child_element("targetPluginBoard", tools_set) xml_child_element("platformTool", tools_set)->children = xml_textnode("PICkit3PlatformToo"
                                                                                                                                                                                                                             "l");
xml_child_element("languageToolchain", tools_set)->children = xml_textnode("XC8");
xml_child_element("languageToolchainVersion", tools_set)->children = xml_textnode("1.45");
xml_child_element("platform", tools_set)->children = xml_textnode("2");
xmlnode* packs = xml_child_element("packs", conf) xmlnode* pack = xml_child_element("pack", packs) pack->attributes = xml_attributes("name", "PIC18Fxxxx_DFP", "vendor", "Microchip", "version", "1.1.19", 0);
xmlnode* compile_type = xml_child_element("compileType", conf) xmlnode* linker_tool = xml_child_element("linkerTool", compile_type) xmlnode* linker_lib_items = xml_child_element("linkerLibItems", linker_tool) xmlnode* archiver_tool = xml_child_element("archiverTool", compile_type) xmlnode* loading = xml_child_element("loading", compile_type) xml_child_element("useAlterna"
                                                                                                                                                                                                                                                                                                                                                                          "teLoadable"
                                                                                                                                                                                                                                                                                                                                                                          "File",
                                                                                                                                                                                                                                                                                                                                                                          loading)
                                                                                                                                                                                                                                                                                                                 ->children = xml_textnode("false");
xml_child_element("parseOnProdLoad", loading)->children = xml_textnode("false");
xmlnode* alternate_loadable_file = xml_child_element("alternateLoadableFile", loading) xmlnode* subordinates = xml_child_element("subordinates", compile_type) xmlnode* make_customization_type = xml_child_element("makeCustomizationT"
                                                                                                                                                                                                                    "ype",
                                                                                                                                                                                                                    conf) xml_child_element("makeCustomizat"
                                                                                                                                                                                                                                            "ionPreStepEnab"
                                                                                                                                                                                                                                            "led",
                                                                                                                                                                                                                                            make_customization_type)
                                                                                                                                                                                                      ->children = xml_textnode("false");
xmlnode* make_customization_pre_step = xml_child_element("makeCustomizationPreStep", make_customization_type) xml_child_element("makeCustomizationPostStepE"
                                                                                                                                "nabled",
                                                                                                                                make_customization_type)
                                           ->children = xml_textnode("false");
xmlnode* make_customization_post_step = xml_child_element("makeCustomizationPostStep", make_customization_type) xml_child_element("makeCustomizationPutChecks"
                                                                                                                                  "umInUserID",
                                                                                                                                  make_customization_type)
                                            ->children = xml_textnode("false");
xml_child_element("makeCustomizationEnableLongLines", make_customization_type)->children = xml_textnode("false");
xml_child_element("makeCustomizationNormalizeHexFile", make_customization_type)->children = xml_textnode("false");
xmlnode* hitechcomp = xml_child_element("HI-TECH-COMP", conf) xmlnode* property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "additional-warnings", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "asmlist", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "default-bitfield-type", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "default-char-type", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key",
                                                                                           "define-macros",
                                                                                           "value",
                                                                                           "__XC=1;__18f2550=1;USE_SER=1;"
                                                                                           "USE_HD44780_LCD=1;_XTAL_FREQ="
                                                                                           "20000000;BAUD_RATE="
                                                                                           "38400;USE_TIMER0=1;"
                                                                                           "MCHP_XC8=1;NDEBUG=1",
                                                                                           0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "disable-optimizations", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key",
                                                                                           "extra-include-directories",
                                                                                           "value",
                                                                                           "../../../src;../../../lib;../"
                                                                                           "../..",
                                                                                           0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "favor-optimization-for", "value", "-speed,+space", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "garbage-collect-data", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "garbage-collect-functions", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "identifier-length", "value", "255", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "local-generation", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "operation-mode", "value", "pro", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "opt-xc8-compiler-strict_ansi", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-assembler", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-assembler-files", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-debug", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-invariant-enable", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-invariant-value", "value", "16", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-level", "value", "-Os", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-speed", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-stable-enable", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "pack-struct", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "preprocess-assembler", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "short-enums", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "undefine-macros", "value", "", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "use-cci", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "use-iar", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "verbose", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "warning-level", "value", "3", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "what-to-do", "value", "ignore", 0);
xmlnode* append_me = xml_child_element("appendMe", hitechcomp) append_me->attributes = xml_attributes("value", "--double=32", 0);
xmlnode* hitechlink = xml_child_element("HI-TECH-LINK", conf) property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-checksum", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "code-offset",
                                                                                           "value",
                                                                                           "",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "command-line",
                                                                                           "value",
                                                                                           "",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-errata", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "extend-address",
                                                                                           "value",
                                                                                           "false",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-trace-type", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "use-response-files",
                                                                                           "value",
                                                                                           "false",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "backup-reset-condition-flags", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "calibrate-oscillator", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "calibrate-oscillator-value", "value", "0x3400", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "clear-bss", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "code-model-external", "value", "wordwrite", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "code-model-rom", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "create-html-files", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-ram", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-double", "value", "32", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-double-gcc", "value", "no-short-double", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-float", "value", "32", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-float-gcc", "value", "no-short-float", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-class-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-hex-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-overall-usage", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-psect-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "extra-lib-directories", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-addr", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-const", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-how", "value", "0", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-inc-const", "value", "1", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-increment", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-seq", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-what", "value", "0", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "format-hex-file-for-download", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "initialize-data", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "input-libraries", "value", "libm", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "keep-generated-startup.as", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-c-library", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-c-library-gcc", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-peripheral-library", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "managed-stack", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-file", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-link_startup", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-serial", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "program-the-device-with-"
                                                                                           "default-config-words",
                                                                                           "value",
                                                                                           "true",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "remove-unused-sections", "value", "true", 0);
xmlnode* pickit3platform_tool = xml_child_element("PICkit3PlatformTool", conf) property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "AutoSelectMemRanges", "value", "auto", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "Freeze Peripherals", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "SecureSegment."
                                                                                                     "SegmentProgramming",
                                                                                                     "value",
                                                                                                     "FullChipProgramming",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "ToolFirmwareFilePath",
                                                                                                     "value",
                                                                                                     "Press to browse for a "
                                                                                                     "specific firmware version",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "ToolFirmwareOption."
                                                                                                     "UseLatestFirmware",
                                                                                                     "value",
                                                                                                     "true",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "debugoptions.useswbreakpoints", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "hwtoolclock.frcindebug", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.aux", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.bootflash", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.configurationmemory", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.configurationmemory2", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.dataflash", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.eeprom", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.flashdata", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.id", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.instruction.ram", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "memories.instruction.ram."
                                                                                                     "ranges",
                                                                                                     "value",
                                                                                                     "${memories.instruction.ram."
                                                                                                     "ranges}",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.programmemory", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.programmemory.ranges", "value", "0-1fff", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "poweroptions.powerenable", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programmertogo.imagename", "value", "", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "donoteraseauxmem",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.eraseb4program", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.pgmspeed", "value", "2", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preservedataflash",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preservedataflash.ranges",
                                                                                                     "value",
                                                                                                     "${programoptions."
                                                                                                     "preservedataflash.ranges}",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.preserveeeprom", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions.preserveeeprom."
                                                                                                     "ranges",
                                                                                                     "value",
                                                                                                     "0-ff",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preserveprogram.ranges",
                                                                                                     "value",
                                                                                                     "",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preserveprogramrange",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.preserveuserid", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.programcalmem", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.programuserotp", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "testmodeentrymethod",
                                                                                                     "value",
                                                                                                     "VDDFirst",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "usehighvoltageonmclr",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "uselvpprogramming",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "voltagevalue", "value", "5.0", 0);
xmlnode* xc8co = xml_child_element("XC8-CO", conf) property = xml_child_element("property", xc8co) property->attributes = xml_attributes("key", "coverage-enable", "value", "", 0);
xmlnode* xc8configglobal = xml_child_element("XC8-config-global", conf) property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "advanced-elf", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-opt-driver-new", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-opt-std", "value", "--std=c89", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-output-file-format", "value", "dwarf-3", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "omit-pack-options", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "output-file-format", "value", "-mcof,+elf", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-high", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-low", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-main", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-type", "value", "compiled", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "user-pack-device-support", "value", "", 0);
conf = xml_child_element("conf", confs) conf->attributes = xml_attributes("name", "debug", "type", "2", 0);
tools_set = xml_child_element("toolsSet", conf) xml_child_element("developmentServer", tools_set)->children = xml_textnode("localhost");
xml_child_element("targetDevice", tools_set)->children = xml_textnode("PIC18F2550");
target_header = xml_child_element("targetHeader", tools_set) target_plugin_board = xml_child_element("targetPluginBoard", tools_set) xml_child_element("platformTool", tools_set)->children = xml_textnode("PICkit3PlatformTool");
xml_child_element("languageToolchain", tools_set)->children = xml_textnode("XC8");
xml_child_element("languageToolchainVersion", tools_set)->children = xml_textnode("1.45");
xml_child_element("platform", tools_set)->children = xml_textnode("2");
packs = xml_child_element("packs", conf) pack = xml_child_element("pack", packs) pack->attributes = xml_attributes("name", "PIC18Fxxxx_DFP", "vendor", "Microchip", "version", "1.1.19", 0);
compile_type = xml_child_element("compileType", conf) linker_tool = xml_child_element("linkerTool", compile_type) linker_lib_items = xml_child_element("linkerLibItems", linker_tool) archiver_tool = xml_child_element("archiverTool", compile_type) loading = xml_child_element("loading", compile_type) xml_child_element("useAlternateLo"
                                                                                                                                                                                                                                                                                                                             "adableFile",
                                                                                                                                                                                                                                                                                                                             loading)
                                                                                                                                                                                                                                                                    ->children = xml_textnode("false");
xml_child_element("parseOnProdLoad", loading)->children = xml_textnode("false");
alternate_loadable_file = xml_child_element("alternateLoadableFile", loading) subordinates = xml_child_element("subordinates", compile_type) make_customization_type = xml_child_element("makeCustomizationType", conf) xml_child_element("makeCustomizationP"
                                                                                                                                                                                                                                          "reStepEnabled",
                                                                                                                                                                                                                                          make_customization_type)
                                                                                                                                                                           ->children = xml_textnode("false");
make_customization_pre_step = xml_child_element("makeCustomizationPreStep", make_customization_type) xml_child_element("makeCustomizationPostStepE"
                                                                                                                       "nabled",
                                                                                                                       make_customization_type)
                                  ->children = xml_textnode("false");
make_customization_post_step = xml_child_element("makeCustomizationPostStep", make_customization_type) xml_child_element("makeCustomizationPutChecks"
                                                                                                                         "umInUserID",
                                                                                                                         make_customization_type)
                                   ->children = xml_textnode("false");
xml_child_element("makeCustomizationEnableLongLines", make_customization_type)->children = xml_textnode("false");
xml_child_element("makeCustomizationNormalizeHexFile", make_customization_type)->children = xml_textnode("false");
hitechcomp = xml_child_element("HI-TECH-COMP", conf) property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "additional-warnings", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "asmlist", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "default-bitfield-type", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "default-char-type", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key",
                                                                                           "define-macros",
                                                                                           "value",
                                                                                           "__XC=1;__18f2550=1;USE_TIMER1="
                                                                                           "1;USE_TIMER2=1;USE_SER=1;USE_"
                                                                                           "HD44780_LCD=1;XTAL_"
                                                                                           "FREQ=16000000;BAUD_"
                                                                                           "RATE=38400;USE_TIMER0=1;MCHP_"
                                                                                           "XC8=1;DEBUG=1",
                                                                                           0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "disable-optimizations", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key",
                                                                                           "extra-include-directories",
                                                                                           "value",
                                                                                           "../../../src;../../../lib;../"
                                                                                           "../..",
                                                                                           0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "favor-optimization-for", "value", "-speed,+space", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "garbage-collect-data", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "garbage-collect-functions", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "identifier-length", "value", "255", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "local-generation", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "operation-mode", "value", "pro", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "opt-xc8-compiler-strict_ansi", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-assembler", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-assembler-files", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-debug", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-invariant-enable", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-invariant-value", "value", "16", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-level", "value", "-O0", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-speed", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "optimization-stable-enable", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "pack-struct", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "preprocess-assembler", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "short-enums", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "undefine-macros", "value", "", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "use-cci", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "use-iar", "value", "false", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "verbose", "value", "true", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "warning-level", "value", "3", 0);
property = xml_child_element("property", hitechcomp) property->attributes = xml_attributes("key", "what-to-do", "value", "ignore", 0);
append_me = xml_child_element("appendMe", hitechcomp) append_me->attributes = xml_attributes("value", "--double=32", 0);
hitechlink = xml_child_element("HI-TECH-LINK", conf) property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-checksum", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "code-offset",
                                                                                           "value",
                                                                                           "",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "command-line",
                                                                                           "value",
                                                                                           "",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-errata", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "extend-address",
                                                                                           "value",
                                                                                           "false",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "additional-options-trace-type", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "additional-options-"
                                                                                           "use-response-files",
                                                                                           "value",
                                                                                           "false",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "backup-reset-condition-flags", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "calibrate-oscillator", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "calibrate-oscillator-value", "value", "0x3400", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "clear-bss", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "code-model-external", "value", "wordwrite", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "code-model-rom", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "create-html-files", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-ram", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-double", "value", "32", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-double-gcc", "value", "no-short-double", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-float", "value", "32", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "data-model-size-of-float-gcc", "value", "no-short-float", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-class-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-hex-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-overall-usage", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "display-psect-usage", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "extra-lib-directories", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-addr", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-const", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-how", "value", "0", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-inc-const", "value", "1", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-increment", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-seq", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "fill-flash-options-what", "value", "0", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "format-hex-file-for-download", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "initialize-data", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "input-libraries", "value", "libm", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "keep-generated-startup.as", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-c-library", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-c-library-gcc", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "link-in-peripheral-library", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "managed-stack", "value", "true", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-file", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-link_startup", "value", "false", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "opt-xc8-linker-serial", "value", "", 0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key",
                                                                                           "program-the-device-with-"
                                                                                           "default-config-words",
                                                                                           "value",
                                                                                           "true",
                                                                                           0);
property = xml_child_element("property", hitechlink) property->attributes = xml_attributes("key", "remove-unused-sections", "value", "true", 0);
pickit3platform_tool = xml_child_element("PICkit3PlatformTool", conf) property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "AutoSelectMemRanges", "value", "auto", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "Freeze Peripherals", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "SecureSegment."
                                                                                                     "SegmentProgramming",
                                                                                                     "value",
                                                                                                     "FullChipProgramming",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "ToolFirmwareFilePath",
                                                                                                     "value",
                                                                                                     "Press to browse for a "
                                                                                                     "specific firmware version",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "ToolFirmwareOption."
                                                                                                     "UseLatestFirmware",
                                                                                                     "value",
                                                                                                     "true",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "debugoptions.useswbreakpoints", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "hwtoolclock.frcindebug", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.aux", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.bootflash", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.configurationmemory", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.configurationmemory2", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.dataflash", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.eeprom", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.flashdata", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.id", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.instruction.ram", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "memories.instruction.ram."
                                                                                                     "ranges",
                                                                                                     "value",
                                                                                                     "${memories.instruction.ram."
                                                                                                     "ranges}",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.programmemory", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "memories.programmemory.ranges", "value", "0-1fff", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "poweroptions.powerenable", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programmertogo.imagename", "value", "", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "donoteraseauxmem",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.eraseb4program", "value", "true", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.pgmspeed", "value", "2", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preservedataflash",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preservedataflash.ranges",
                                                                                                     "value",
                                                                                                     "${programoptions."
                                                                                                     "preservedataflash.ranges}",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.preserveeeprom", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions.preserveeeprom."
                                                                                                     "ranges",
                                                                                                     "value",
                                                                                                     "0-ff",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preserveprogram.ranges",
                                                                                                     "value",
                                                                                                     "",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "preserveprogramrange",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.preserveuserid", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.programcalmem", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "programoptions.programuserotp", "value", "false", 0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "testmodeentrymethod",
                                                                                                     "value",
                                                                                                     "VDDFirst",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "usehighvoltageonmclr",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key",
                                                                                                     "programoptions."
                                                                                                     "uselvpprogramming",
                                                                                                     "value",
                                                                                                     "false",
                                                                                                     0);
property = xml_child_element("property", pickit3platform_tool) property->attributes = xml_attributes("key", "voltagevalue", "value", "5.0", 0);
xc8co = xml_child_element("XC8-CO", conf) property = xml_child_element("property", xc8co) property->attributes = xml_attributes("key", "coverage-enable", "value", "", 0);
xc8configglobal = xml_child_element("XC8-config-global", conf) property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "advanced-elf", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-opt-driver-new", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-opt-std", "value", "--std=c89", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "gcc-output-file-format", "value", "dwarf-3", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "omit-pack-options", "value", "false", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "output-file-format", "value", "+mcof,-elf", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-high", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-low", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-size-main", "value", "auto", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "stack-type", "value", "compiled", 0);
property = xml_child_element("property", xc8configglobal) property->attributes = xml_attributes("key", "user-pack-device-support", "value", "", 0);
