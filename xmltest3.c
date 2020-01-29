#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/xml.h"

xmlnode*
create_xml_document() {
  xmlnode* doc = xml_newnode(XML_DOCUMENT);
  xmlnode* configuration_descriptor = doc->children = xml_element("configurationDescriptor");

  configuration_descriptor->attributes = xml_attributes("version", "65", 0);

  xmlnode* logical_folder = xml_child_element("logicalFolder", configuration_descriptor);
  logical_folder->attributes = xml_attributes("name", "root", "displayName", "root", "projectFiles", "true", 0);

  xmlnode* logical_folder1 = xml_child_element("logicalFolder", logical_folder);
  logical_folder->attributes =
      xml_attributes("name", "HeaderFiles", "displayName", "Header Files", "projectFiles", "true", 0);

  xmlnode* logical_folder2 = xml_child_element("logicalFolder", logical_folder1);
  logical_folder->attributes = xml_attributes("name", "f1", "displayName", "lib", "projectFiles", "true", 0);

  xml_child_element_text("itemPath", logical_folder2, "../../../lib/buffer.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/interrupt.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/format.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/lcd44780.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/oscillator.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/ser.h");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/typedef.h");
  xml_child_element_text("itemPath", logical_folder2, "/home/roman/Dokumente/Sources/lc-meter/lib/delay.h");

  xml_child_element_attrs(
      "logicalFolder", logical_folder, "name=LinkerScript", "displayName=Linker Files", "projectFiles=true", 0);
  xmlnode* logical_folder3 = xml_child_element("logicalFolder", logical_folder);
  logical_folder->attributes =
      xml_attributes("name", "SourceFiles", "displayName", "Source Files", "projectFiles", "true", 0);

  xmlnode* logical_folder4 = xml_child_element("logicalFolder", logical_folder3);
  logical_folder->attributes = xml_attributes("name", "f1", "displayName", "lib", "projectFiles", "true", 0);

  xml_child_element_text("itemPath", logical_folder4, "../../../lib/buffer.c");
  xml_child_element_text("itemPath", logical_folder4, "/home/roman/Dokumente/Sources/lc-meter/lib/delay.c");
  xml_child_element_text("itemPath", logical_folder4, "../../../lib/lcd44780.c");
  xml_child_element_text("itemPath", logical_folder4, "../../../lib/ser.c");
  xml_child_element_text("itemPath", logical_folder4, "../../../lib/timer.c");
  xml_child_element_text("itemPath", logical_folder4, "../../../lib/format.c");

  xml_child_element_text("itemPath", logical_folder3, "../../../src/print.c");
  xml_child_element_text("itemPath", logical_folder3, "../../../LC-meter.c");
  xml_child_element_text("itemPath", logical_folder3, "../../../src/measure.c");

  xmlnode* logical_folder5 = xml_child_element("logicalFolder", logical_folder);
  logical_folder->attributes =
      xml_attributes("name", "ExternalFiles", "displayName", "Important Files", "projectFiles", "false", 0);

  xml_child_element_text("itemPath", logical_folder5, "Makefile");

  xmlnode* source_root_list = xml_child_element("sourceRootList", configuration_descriptor);

  xml_child_element_text("Elem", source_root_list, "../../mplab");
  xml_child_element_text("Elem", source_root_list, "../../../src");
  xml_child_element_text("Elem", source_root_list, "../../../lib");

  xml_child_element_text("projectmakefile", configuration_descriptor, "Makefile");
  xmlnode* confs = xml_child_element("confs", configuration_descriptor);

  xmlnode* conf = xml_child_element("conf", confs);
  conf->attributes = xml_attributes("name", "release", "type", "2", 0);

  xmlnode* tools_set = xml_child_element("toolsSet", conf);

  xml_child_element_text("developmentServer", tools_set, "localhost");
  xml_child_element_text("targetDevice", tools_set, "PIC18F2550");
  xml_child_element("targetHeader", tools_set);
  xml_child_element("targetPluginBoard", tools_set);
  xml_child_element_text("platformTool", tools_set, "PICkit3PlatformTool");
  xml_child_element_text("languageToolchain", tools_set, "XC8");
  xml_child_element_text("languageToolchainVersion", tools_set, "1.45");
  xml_child_element_text("platform", tools_set, "2");

  xmlnode* packs = xml_child_element("packs", conf);

  xml_child_element_attrs("pack", packs, "name=PIC18Fxxxx_DFP", "vendor=Microchip", "version=1.1.19", 0);

  xmlnode* compile_type = xml_child_element("compileType", conf);

  xmlnode* linker_tool = xml_child_element("linkerTool", compile_type);

  xml_child_element("linkerLibItems", linker_tool);

  xml_child_element("archiverTool", compile_type);
  xmlnode* loading = xml_child_element("loading", compile_type);

  xml_child_element_text("useAlternateLoadableFile", loading, "false");
  xml_child_element_text("parseOnProdLoad", loading, "false");
  xml_child_element("alternateLoadableFile", loading);

  xml_child_element("subordinates", compile_type);

  xmlnode* make_customization_type = xml_child_element("makeCustomizationType", conf);

  xml_child_element_text("makeCustomizationPreStepEnabled", make_customization_type, "false");
  xml_child_element("makeCustomizationPreStep", make_customization_type);
  xml_child_element_text("makeCustomizationPostStepEnabled", make_customization_type, "false");
  xml_child_element("makeCustomizationPostStep", make_customization_type);
  xml_child_element_text("makeCustomizationPutChecksumInUserID", make_customization_type, "false");
  xml_child_element_text("makeCustomizationEnableLongLines", make_customization_type, "false");
  xml_child_element_text("makeCustomizationNormalizeHexFile", make_customization_type, "false");

  xmlnode* hitechcomp = xml_child_element("HI-TECH-COMP", conf);

  xml_child_element_attrs("property", hitechcomp, "key=additional-warnings", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=asmlist", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=default-bitfield-type", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=default-char-type", "value=true", 0);
  xml_child_element_attrs("property",
                          hitechcomp,
                          "key=define-macros",
                          "value=__XC=1;__18f2550=1;USE_SER=1;USE_HD44780_LCD=1;_XTAL_FREQ=20000000;BAUD_RATE=38400;"
                          "USE_TIMER0=1;MCHP_XC8=1;NDEBUG=1",
                          0);
  xml_child_element_attrs("property", hitechcomp, "key=disable-optimizations", "value=false", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key=extra-include-directories", "value=../../../src;../../../lib;../../..", 0);
  xml_child_element_attrs("property", hitechcomp, "key=favor-optimization-for", "value=-speed,+space", 0);
  xml_child_element_attrs("property", hitechcomp, "key=garbage-collect-data", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=garbage-collect-functions", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=identifier-length", "value=255", 0);
  xml_child_element_attrs("property", hitechcomp, "key=local-generation", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=operation-mode", "value=pro", 0);
  xml_child_element_attrs("property", hitechcomp, "key=opt-xc8-compiler-strict_ansi", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-assembler", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-assembler-files", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-debug", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-invariant-enable", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-invariant-value", "value=16", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-level", "value=-Os", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-speed", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=optimization-stable-enable", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=pack-struct", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=preprocess-assembler", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=short-enums", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=undefine-macros", "value=", 0);
  xml_child_element_attrs("property", hitechcomp, "key=use-cci", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=use-iar", "value=false", 0);
  xml_child_element_attrs("property", hitechcomp, "key=verbose", "value=true", 0);
  xml_child_element_attrs("property", hitechcomp, "key=warning-level", "value=3", 0);
  xml_child_element_attrs("property", hitechcomp, "key=what-to-do", "value=ignore", 0);
  xml_child_element_attrs("appendMe", hitechcomp, "value=--double=32", 0);

  xmlnode* hitechlink = xml_child_element("HI-TECH-LINK", conf);

  xml_child_element_attrs("property", hitechlink, "key=additional-options-checksum", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-code-offset", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-command-line", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-errata", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-extend-address", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-trace-type", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=additional-options-use-response-files", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=backup-reset-condition-flags", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=calibrate-oscillator", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=calibrate-oscillator-value", "value=0x3400", 0);
  xml_child_element_attrs("property", hitechlink, "key=clear-bss", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=code-model-external", "value=wordwrite", 0);
  xml_child_element_attrs("property", hitechlink, "key=code-model-rom", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=create-html-files", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=data-model-ram", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=data-model-size-of-double", "value=32", 0);
  xml_child_element_attrs("property", hitechlink, "key=data-model-size-of-double-gcc", "value=no-short-double", 0);
  xml_child_element_attrs("property", hitechlink, "key=data-model-size-of-float", "value=32", 0);
  xml_child_element_attrs("property", hitechlink, "key=data-model-size-of-float-gcc", "value=no-short-float", 0);
  xml_child_element_attrs("property", hitechlink, "key=display-class-usage", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=display-hex-usage", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=display-overall-usage", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=display-psect-usage", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=extra-lib-directories", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-addr", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-const", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-how", "value=0", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-inc-const", "value=1", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-increment", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-seq", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=fill-flash-options-what", "value=0", 0);
  xml_child_element_attrs("property", hitechlink, "key=format-hex-file-for-download", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=initialize-data", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=input-libraries", "value=libm", 0);
  xml_child_element_attrs("property", hitechlink, "key=keep-generated-startup.as", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=link-in-c-library", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=link-in-c-library-gcc", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=link-in-peripheral-library", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=managed-stack", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=opt-xc8-linker-file", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=opt-xc8-linker-link_startup", "value=false", 0);
  xml_child_element_attrs("property", hitechlink, "key=opt-xc8-linker-serial", "value=", 0);
  xml_child_element_attrs("property", hitechlink, "key=program-the-device-with-default-config-words", "value=true", 0);
  xml_child_element_attrs("property", hitechlink, "key=remove-unused-sections", "value=true", 0);

  xmlnode* pickit3platform_tool = xml_child_element("PICkit3PlatformTool", conf);

  xml_child_element_attrs("property", pickit3platform_tool, "key=AutoSelectMemRanges", "value=auto", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=Freeze Peripherals", "value=true", 0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=SecureSegment.SegmentProgramming", "value=FullChipProgramming", 0);
  xml_child_element_attrs("property",
                          pickit3platform_tool,
                          "key=ToolFirmwareFilePath",
                          "value=Press to browse for a specific firmware version",
                          0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=ToolFirmwareOption.UseLatestFirmware", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=debugoptions.useswbreakpoints", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=hwtoolclock.frcindebug", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.aux", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.bootflash", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.configurationmemory", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.configurationmemory2", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.dataflash", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.eeprom", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.flashdata", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.id", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.instruction.ram", "value=true", 0);
  xml_child_element_attrs("property",
                          pickit3platform_tool,
                          "key=memories.instruction.ram.ranges",
                          "value=${memories.instruction.ram.ranges}",
                          0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.programmemory", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=memories.programmemory.ranges", "value=0-1fff", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=poweroptions.powerenable", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programmertogo.imagename", "value=", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.donoteraseauxmem", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.eraseb4program", "value=true", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.pgmspeed", "value=2", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.preservedataflash", "value=false", 0);
  xml_child_element_attrs("property",
                          pickit3platform_tool,
                          "key=programoptions.preservedataflash.ranges",
                          "value=${programoptions.preservedataflash.ranges}",
                          0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.preserveeeprom", "value=false", 0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=programoptions.preserveeeprom.ranges", "value=0-ff", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.preserveprogram.ranges", "value=", 0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=programoptions.preserveprogramrange", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.preserveuserid", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.programcalmem", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.programuserotp", "value=false", 0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=programoptions.testmodeentrymethod", "value=VDDFirst", 0);
  xml_child_element_attrs(
      "property", pickit3platform_tool, "key=programoptions.usehighvoltageonmclr", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=programoptions.uselvpprogramming", "value=false", 0);
  xml_child_element_attrs("property", pickit3platform_tool, "key=voltagevalue", "value=5.0", 0);

  xmlnode* xc8co = xml_child_element("XC8-CO", conf);

  xml_child_element_attrs("property", xc8co, "key=coverage-enable", "value=", 0);

  xmlnode* xc8configglobal = xml_child_element("XC8-config-global", conf);

  xml_child_element_attrs("property", xc8configglobal, "key=advanced-elf", "value=false", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=gcc-opt-driver-new", "value=false", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=gcc-opt-std", "value=--std=c89", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=gcc-output-file-format", "value=dwarf-3", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=omit-pack-options", "value=false", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=output-file-format", "value=-mcof,+elf", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=stack-size-high", "value=auto", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=stack-size-low", "value=auto", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=stack-size-main", "value=auto", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=stack-type", "value=compiled", 0);
  xml_child_element_attrs("property", xc8configglobal, "key=user-pack-device-support", "value=", 0);

  xmlnode* conf1 = xml_child_element("conf", confs);
  conf1->attributes = xml_attributes("name", "debug", "type", "2", 0);

  xmlnode* tool2 = xml_child_element("toolsSet", conf1);

  xml_child_element_text("developmentServer", tool2, "localhost");
  xml_child_element_text("targetDevice", tool2, "PIC18F2550");
  xml_child_element("targetHeader", tool2);
  xml_child_element("targetPluginBoard", tool2);
  xml_child_element_text("platformTool", tool2, "PICkit3PlatformTool");
  xml_child_element_text("languageToolchain", tool2, "XC8");
  xml_child_element_text("languageToolchainVersion", tool2, "1.45");
  xml_child_element_text("platform", tool2, "2");

  xmlnode* pack2 = xml_child_element("packs", conf1);

  xml_child_element_attrs("pack", pack2, "name=PIC18Fxxxx_DFP", "vendor=Microchip", "version=1.1.19", 0);

  xmlnode* comp2 = xml_child_element("compileType", conf1);

  xmlnode* link2 = xml_child_element("linkerTool", comp2);

  xml_child_element("linkerLibItems", link2);

  xml_child_element("archiverTool", comp2);
  xmlnode* load2 = xml_child_element("loading", comp2);

  xml_child_element_text("useAlternateLoadableFile", load2, "false");
  xml_child_element_text("parseOnProdLoad", load2, "false");
  xml_child_element("alternateLoadableFile", load2);

  xml_child_element("subordinates", comp2);

  xmlnode* make2 = xml_child_element("makeCustomizationType", conf1);

  xml_child_element_text("makeCustomizationPreStepEnabled", make2, "false");
  xml_child_element("makeCustomizationPreStep", make2);
  xml_child_element_text("makeCustomizationPostStepEnabled", make2, "false");
  xml_child_element("makeCustomizationPostStep", make2);
  xml_child_element_text("makeCustomizationPutChecksumInUserID", make2, "false");
  xml_child_element_text("makeCustomizationEnableLongLines", make2, "false");
  xml_child_element_text("makeCustomizationNormalizeHexFile", make2, "false");

  xmlnode* hite2 = xml_child_element("HI-TECH-COMP", conf1);

  xml_child_element_attrs("property", hite2, "key=additional-warnings", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=asmlist", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=default-bitfield-type", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=default-char-type", "value=true", 0);
  xml_child_element_attrs("property",
                          hite2,
                          "key=define-macros",
                          "value=__XC=1;__18f2550=1;USE_TIMER1=1;USE_TIMER2=1;USE_SER=1;USE_HD44780_LCD=1;XTAL_FREQ="
                          "16000000;BAUD_RATE=38400;USE_TIMER0=1;MCHP_XC8=1;DEBUG=1",
                          0);
  xml_child_element_attrs("property", hite2, "key=disable-optimizations", "value=true", 0);
  xml_child_element_attrs(
      "property", hite2, "key=extra-include-directories", "value=../../../src;../../../lib;../../..", 0);
  xml_child_element_attrs("property", hite2, "key=favor-optimization-for", "value=-speed,+space", 0);
  xml_child_element_attrs("property", hite2, "key=garbage-collect-data", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=garbage-collect-functions", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=identifier-length", "value=255", 0);
  xml_child_element_attrs("property", hite2, "key=local-generation", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=operation-mode", "value=pro", 0);
  xml_child_element_attrs("property", hite2, "key=opt-xc8-compiler-strict_ansi", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-assembler", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-assembler-files", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-debug", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-invariant-enable", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-invariant-value", "value=16", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-level", "value=-O0", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-speed", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=optimization-stable-enable", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=pack-struct", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=preprocess-assembler", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=short-enums", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=undefine-macros", "value=", 0);
  xml_child_element_attrs("property", hite2, "key=use-cci", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=use-iar", "value=false", 0);
  xml_child_element_attrs("property", hite2, "key=verbose", "value=true", 0);
  xml_child_element_attrs("property", hite2, "key=warning-level", "value=3", 0);
  xml_child_element_attrs("property", hite2, "key=what-to-do", "value=ignore", 0);
  xml_child_element_attrs("appendMe", hite2, "value=--double=32", 0);

  xmlnode* hite3 = xml_child_element("HI-TECH-LINK", conf1);

  xml_child_element_attrs("property", hite3, "key=additional-options-checksum", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-code-offset", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-command-line", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-errata", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-extend-address", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-trace-type", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=additional-options-use-response-files", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=backup-reset-condition-flags", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=calibrate-oscillator", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=calibrate-oscillator-value", "value=0x3400", 0);
  xml_child_element_attrs("property", hite3, "key=clear-bss", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=code-model-external", "value=wordwrite", 0);
  xml_child_element_attrs("property", hite3, "key=code-model-rom", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=create-html-files", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=data-model-ram", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=data-model-size-of-double", "value=32", 0);
  xml_child_element_attrs("property", hite3, "key=data-model-size-of-double-gcc", "value=no-short-double", 0);
  xml_child_element_attrs("property", hite3, "key=data-model-size-of-float", "value=32", 0);
  xml_child_element_attrs("property", hite3, "key=data-model-size-of-float-gcc", "value=no-short-float", 0);
  xml_child_element_attrs("property", hite3, "key=display-class-usage", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=display-hex-usage", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=display-overall-usage", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=display-psect-usage", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=extra-lib-directories", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-addr", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-const", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-how", "value=0", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-inc-const", "value=1", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-increment", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-seq", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=fill-flash-options-what", "value=0", 0);
  xml_child_element_attrs("property", hite3, "key=format-hex-file-for-download", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=initialize-data", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=input-libraries", "value=libm", 0);
  xml_child_element_attrs("property", hite3, "key=keep-generated-startup.as", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=link-in-c-library", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=link-in-c-library-gcc", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=link-in-peripheral-library", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=managed-stack", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=opt-xc8-linker-file", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=opt-xc8-linker-link_startup", "value=false", 0);
  xml_child_element_attrs("property", hite3, "key=opt-xc8-linker-serial", "value=", 0);
  xml_child_element_attrs("property", hite3, "key=program-the-device-with-default-config-words", "value=true", 0);
  xml_child_element_attrs("property", hite3, "key=remove-unused-sections", "value=true", 0);

  xmlnode* pick2 = xml_child_element("PICkit3PlatformTool", conf1);

  xml_child_element_attrs("property", pick2, "key=AutoSelectMemRanges", "value=auto", 0);
  xml_child_element_attrs("property", pick2, "key=Freeze Peripherals", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=SecureSegment.SegmentProgramming", "value=FullChipProgramming", 0);
  xml_child_element_attrs(
      "property", pick2, "key=ToolFirmwareFilePath", "value=Press to browse for a specific firmware version", 0);
  xml_child_element_attrs("property", pick2, "key=ToolFirmwareOption.UseLatestFirmware", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=debugoptions.useswbreakpoints", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=hwtoolclock.frcindebug", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=memories.aux", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=memories.bootflash", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.configurationmemory", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.configurationmemory2", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.dataflash", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.eeprom", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.flashdata", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.id", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.instruction.ram", "value=true", 0);
  xml_child_element_attrs(
      "property", pick2, "key=memories.instruction.ram.ranges", "value=${memories.instruction.ram.ranges}", 0);
  xml_child_element_attrs("property", pick2, "key=memories.programmemory", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=memories.programmemory.ranges", "value=0-1fff", 0);
  xml_child_element_attrs("property", pick2, "key=poweroptions.powerenable", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programmertogo.imagename", "value=", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.donoteraseauxmem", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.eraseb4program", "value=true", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.pgmspeed", "value=2", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preservedataflash", "value=false", 0);
  xml_child_element_attrs("property",
                          pick2,
                          "key=programoptions.preservedataflash.ranges",
                          "value=${programoptions.preservedataflash.ranges}",
                          0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preserveeeprom", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preserveeeprom.ranges", "value=0-ff", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preserveprogram.ranges", "value=", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preserveprogramrange", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.preserveuserid", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.programcalmem", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.programuserotp", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.testmodeentrymethod", "value=VDDFirst", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.usehighvoltageonmclr", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=programoptions.uselvpprogramming", "value=false", 0);
  xml_child_element_attrs("property", pick2, "key=voltagevalue", "value=5.0", 0);

  xmlnode* xc8c2 = xml_child_element("XC8-CO", conf1);

  xml_child_element_attrs("property", xc8c2, "key=coverage-enable", "value=", 0);

  xmlnode* xc8c3 = xml_child_element("XC8-config-global", conf1);

  xml_child_element_attrs("property", xc8c3, "key=advanced-elf", "value=false", 0);
  xml_child_element_attrs("property", xc8c3, "key=gcc-opt-driver-new", "value=false", 0);
  xml_child_element_attrs("property", xc8c3, "key=gcc-opt-std", "value=--std=c89", 0);
  xml_child_element_attrs("property", xc8c3, "key=gcc-output-file-format", "value=dwarf-3", 0);
  xml_child_element_attrs("property", xc8c3, "key=omit-pack-options", "value=false", 0);
  xml_child_element_attrs("property", xc8c3, "key=output-file-format", "value=+mcof,-elf", 0);
  xml_child_element_attrs("property", xc8c3, "key=stack-size-high", "value=auto", 0);
  xml_child_element_attrs("property", xc8c3, "key=stack-size-low", "value=auto", 0);
  xml_child_element_attrs("property", xc8c3, "key=stack-size-main", "value=auto", 0);
  xml_child_element_attrs("property", xc8c3, "key=stack-type", "value=compiled", 0);
  xml_child_element_attrs("property", xc8c3, "key=user-pack-device-support", "value=", 0);

  return doc;
}
int
main(int argc, char* argv[1]) {
  xmlnode* doc;
  buffer out;
  byte_zero(&out, sizeof(out));
  buffer_truncfile(&out, "configuration.xml");
  doc = create_xml_document();
  xml_print(doc, buffer_1);
  xml_free(doc);
}