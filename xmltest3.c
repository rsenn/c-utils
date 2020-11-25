#include "lib/scan.h"
#include "lib/byte.h"
#include "lib/xml.h"

xmlnode*
create_xml_document() {
  xmlnode *compile_type, *conf, *configuration_descriptor, *confs, *doc, *hitechcomp, *linker_tool,
      *loading, *logical_folder, *logical_folder1, *logical_folder2, *logical_folder3,
      *make_customization_type, *packs, *source_root_list, *tools_set;

  doc = xml_newnode(XML_DOCUMENT);
  configuration_descriptor = doc->children = xml_element("configurationDescriptor");

  configuration_descriptor->attributes = xml_attributes("version", "65", 0);

  logical_folder = xml_child_element("logicalFolder", configuration_descriptor);
  logical_folder->attributes =
      xml_attributes("name", "root", "displayName", "root", "projectFiles", "true", 0);

  xml_child_element_attrs("logicalFolder",
                          logical_folder,
                          "name",
                          "HeaderFiles",
                          "displayName",
                          "Header Files",
                          "projectFiles",
                          "true",
                          0);
  xml_child_element_attrs("logicalFolder",
                          logical_folder,
                          "name",
                          "LinkerScript",
                          "displayName",
                          "Linker Files",
                          "projectFiles",
                          "true",
                          0);
  logical_folder1 = xml_child_element("logicalFolder", logical_folder);
  logical_folder->attributes = xml_attributes(
      "name", "SourceFiles", "displayName", "Source Files", "projectFiles", "true", 0);

  logical_folder2 = xml_child_element("logicalFolder", logical_folder1);
  logical_folder->attributes =
      xml_attributes("name", "f1", "displayName", "lib", "projectFiles", "true", 0);

  xml_child_element_text("itemPath", logical_folder2, "../../../lib/comparator.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/delay.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/pwm.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/random.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/ser.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/softpwm.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/softser.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/timer.c");
  xml_child_element_text("itemPath", logical_folder2, "../../../lib/uart.c");

  xml_child_element_text("itemPath", logical_folder1, "../../../blinktest.c");

  logical_folder3 = xml_child_element("logicalFolder", logical_folder);
  logical_folder->attributes = xml_attributes(
      "name", "ExternalFiles", "displayName", "Important Files", "projectFiles", "false", 0);

  xml_child_element_text("itemPath", logical_folder3, "Makefile");

  source_root_list = xml_child_element("sourceRootList", configuration_descriptor);

  xml_child_element_text("Elem", source_root_list, "../../..");

  xml_child_element_text("projectmakefile", configuration_descriptor, "Makefile");
  confs = xml_child_element("confs", configuration_descriptor);

  conf = xml_child_element("conf", confs);
  conf->attributes = xml_attributes("name", "debug", "type", "2", 0);

  tools_set = xml_child_element("toolsSet", conf);

  xml_child_element_text("developmentServer", tools_set, "localhost");
  xml_child_element_text("targetDevice", tools_set, "PIC18F2550");
  xml_child_element("targetHeader", tools_set);
  xml_child_element("targetPluginBoard", tools_set);
  xml_child_element_text("platformTool", tools_set, "PICkit3PlatformTool");
  xml_child_element_text("languageToolchain", tools_set, "XC8");
  xml_child_element_text("languageToolchainVersion", tools_set, "1.45");
  xml_child_element_text("platform", tools_set, "3");

  packs = xml_child_element("packs", conf);

  xml_child_element_attrs(
      "pack", packs, "name", "PIC18Fxxxx_DFP", "vendor", "Microchip", "version", "1.1.19", 0);

  compile_type = xml_child_element("compileType", conf);

  linker_tool = xml_child_element("linkerTool", compile_type);

  xml_child_element("linkerLibItems", linker_tool);

  xml_child_element("archiverTool", compile_type);
  loading = xml_child_element("loading", compile_type);

  xml_child_element_text("useAlternateLoadableFile", loading, "false");
  xml_child_element_text("parseOnProdLoad", loading, "false");
  xml_child_element("alternateLoadableFile", loading);

  xml_child_element("subordinates", compile_type);

  make_customization_type = xml_child_element("makeCustomizationType", conf);

  xml_child_element_text("makeCustomizationPreStepEnabled", make_customization_type, "false");
  xml_child_element("makeCustomizationPreStep", make_customization_type);
  xml_child_element_text("makeCustomizationPostStepEnabled", make_customization_type, "false");
  xml_child_element("makeCustomizationPostStep", make_customization_type);
  xml_child_element_text("makeCustomizationPutChecksumInUserID", make_customization_type, "false");
  xml_child_element_text("makeCustomizationEnableLongLines", make_customization_type, "false");
  xml_child_element_text("makeCustomizationNormalizeHexFile", make_customization_type, "false");

  hitechcomp = xml_child_element("HI-TECH-COMP", conf);

  xml_child_element_attrs("property", hitechcomp, "key", "additional-warnings", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "asmlist", "value", "true", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "default-bitfield-type", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "default-char-type", "value", "true", 0);
  xml_child_element_attrs(
      "property",
      hitechcomp,
      "key",
      "define-macros",
      "value",
      "__18f2550=1;XTAL_FREQ=20000000;UART_BAUD=38400;USE_TIMER0=1;USE_TIMER1=1;USE_TIMER2=1;USE_"
      "SOFTPWM=1;USE_SOFTSER=1;HAVE_COMPARATOR=1",
      0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "disable-optimizations", "value", "false", 0);
  xml_child_element_attrs("property",
                          hitechcomp,
                          "key",
                          "extra-include-directories",
                          "value",
                          "../../..;../../../lib;../../../src",
                          0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "favor-optimization-for", "value", "-speed,+space", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "garbage-collect-data", "value", "true", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "garbage-collect-functions", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "identifier-length", "value", "255", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "local-generation", "value", "false", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "operation-mode", "value", "pro", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "opt-xc8-compiler-strict_ansi", "value", "false", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "optimization-assembler", "value", "true", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "optimization-assembler-files", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "optimization-debug", "value", "true", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "optimization-invariant-enable", "value", "false", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "optimization-invariant-value", "value", "16", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "optimization-level", "value", "-Os", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "optimization-speed", "value", "false", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "optimization-stable-enable", "value", "false", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "pack-struct", "value", "true", 0);
  xml_child_element_attrs(
      "property", hitechcomp, "key", "preprocess-assembler", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "short-enums", "value", "true", 0);
  xml_child_element_attrs("property", hitechcomp, "key", "undefine-macros", "value", "", 0);

  return doc;
}
int
main(int argc, char* argv[1]) {
  xmlnode* doc;
  buffer out;
  byte_zero(&out, sizeof(out));
  buffer_truncfile(&out, "configuration.xml");
  doc = create_xml_document();
  xml_print(doc, buffer_1, 0);
  xml_free(doc);
}
