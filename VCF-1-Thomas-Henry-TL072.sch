<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.6.0">
  <drawing>
    <settings>
      <setting alwaysvectorfont="no"/>
      <setting verticaltext="up"/>
    </settings>
    <grid distance="0.05" unitdist="inch" unit="inch" style="lines" multiple="2" display="yes" altdistance="0.025" altunitdist="inch" altunit="inch"/>
    <layers>
      <layer number="1" name="Top" color="4" fill="1" visible="yes" active="no"/>
      <layer number="2" name="Route2" color="1" fill="3" visible="yes" active="no"/>
      <layer number="3" name="Route3" color="4" fill="3" visible="yes" active="no"/>
      <layer number="4" name="Route4" color="1" fill="4" visible="yes" active="no"/>
      <layer number="5" name="Route5" color="4" fill="4" visible="yes" active="no"/>
      <layer number="6" name="Route6" color="1" fill="8" visible="yes" active="no"/>
      <layer number="7" name="Route7" color="4" fill="8" visible="yes" active="no"/>
      <layer number="8" name="Route8" color="1" fill="2" visible="yes" active="no"/>
      <layer number="9" name="Route9" color="4" fill="2" visible="yes" active="no"/>
      <layer number="10" name="Route10" color="1" fill="7" visible="yes" active="no"/>
      <layer number="11" name="Route11" color="4" fill="7" visible="yes" active="no"/>
      <layer number="12" name="Route12" color="1" fill="5" visible="yes" active="no"/>
      <layer number="13" name="Route13" color="4" fill="5" visible="yes" active="no"/>
      <layer number="14" name="Route14" color="1" fill="6" visible="yes" active="no"/>
      <layer number="15" name="Route15" color="4" fill="6" visible="yes" active="no"/>
      <layer number="16" name="Bottom" color="1" fill="1" visible="yes" active="no"/>
      <layer number="17" name="Pads" color="2" fill="1" visible="yes" active="no"/>
      <layer number="18" name="Vias" color="2" fill="1" visible="yes" active="no"/>
      <layer number="19" name="Unrouted" color="6" fill="1" visible="yes" active="no"/>
      <layer number="20" name="Dimension" color="15" fill="1" visible="yes" active="no"/>
      <layer number="21" name="tPlace" color="7" fill="1" visible="yes" active="no"/>
      <layer number="22" name="bPlace" color="7" fill="1" visible="yes" active="no"/>
      <layer number="23" name="tOrigins" color="15" fill="1" visible="yes" active="no"/>
      <layer number="24" name="bOrigins" color="15" fill="1" visible="yes" active="no"/>
      <layer number="25" name="tNames" color="7" fill="1" visible="yes" active="no"/>
      <layer number="26" name="bNames" color="7" fill="1" visible="yes" active="no"/>
      <layer number="27" name="tValues" color="7" fill="1" visible="yes" active="no"/>
      <layer number="28" name="bValues" color="7" fill="1" visible="yes" active="no"/>
      <layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
      <layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
      <layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
      <layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
      <layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
      <layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
      <layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
      <layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
      <layer number="37" name="tTest" color="7" fill="1" visible="yes" active="no"/>
      <layer number="38" name="bTest" color="7" fill="1" visible="yes" active="no"/>
      <layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
      <layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
      <layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
      <layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
      <layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
      <layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
      <layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
      <layer number="46" name="Milling" color="3" fill="1" visible="yes" active="no"/>
      <layer number="47" name="Measures" color="7" fill="1" visible="yes" active="no"/>
      <layer number="48" name="Document" color="7" fill="1" visible="yes" active="no"/>
      <layer number="49" name="Reference" color="7" fill="1" visible="yes" active="no"/>
      <layer number="51" name="tDocu" color="7" fill="1" visible="yes" active="no"/>
      <layer number="52" name="bDocu" color="7" fill="1" visible="yes" active="no"/>
      <layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
      <layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
      <layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
      <layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
      <layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
      <layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
      <layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
      <layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
      <layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
      <layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
    </layers>
    <schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
      <libraries>
        <library name="pad">
          <packages>
            <package name="SPAD+-">
              <pad name="1" x="0" y="-1.27" drill="1" diameter="1.778" stop="no"/>
              <text x="0" y="-2.794" size="0.8128" layer="21" ratio="9" distance="5" align="top-center">&gt;NAME</text>
              <pad name="2" x="0" y="1.27" drill="1" diameter="1.778" stop="no"/>
              <wire x1="-1.27" y1="2.54" x2="-1.27" y2="-2.54" width="0.127" layer="21"/>
              <wire x1="-1.27" y1="-2.54" x2="1.27" y2="-2.54" width="0.127" layer="21"/>
              <wire x1="1.27" y1="-2.54" x2="1.27" y2="2.54" width="0.127" layer="21"/>
              <wire x1="1.27" y1="2.54" x2="-1.27" y2="2.54" width="0.127" layer="21"/>
              <text x="1.905" y="-1.27" size="1.016" layer="21" rot="R180" align="center">-</text>
              <text x="1.905" y="1.27" size="1.016" layer="21" rot="R90" align="center">+</text>
            </package>
            <package name="SPAD-+">
              <pad name="1" x="0" y="-1.27" drill="1" diameter="1.778" stop="no"/>
              <text x="0" y="-2.794" size="0.8128" layer="21" ratio="9" distance="5" align="top-center">&gt;NAME</text>
              <pad name="2" x="0" y="1.27" drill="1" diameter="1.778" stop="no"/>
              <wire x1="-1.27" y1="2.54" x2="-1.27" y2="-2.54" width="0.127" layer="21"/>
              <wire x1="-1.27" y1="-2.54" x2="1.27" y2="-2.54" width="0.127" layer="21"/>
              <wire x1="1.27" y1="-2.54" x2="1.27" y2="2.54" width="0.127" layer="21"/>
              <wire x1="1.27" y1="2.54" x2="-1.27" y2="2.54" width="0.127" layer="21"/>
              <text x="-1.905" y="-1.27" size="1.016" layer="21" rot="R180" align="center">-</text>
              <text x="-1.905" y="1.27" size="1.016" layer="21" rot="R90" align="center">+</text>
            </package>
          </packages>
          <symbols>
            <symbol name="SPAD">
              <pin name="P$1" x="0" y="0" visible="off" length="point" direction="pas" function="dot"/>
              <wire x1="0" y1="0" x2="-2.54" y2="0" width="0.254" layer="94"/>
              <circle x="-3.81" y="0" radius="1.27" width="0.254" layer="94"/>
              <text x="-6.35" y="0" size="1.27" layer="95" align="center-right">&gt;NAME</text>
            </symbol>
            <symbol name="GND">
              <wire x1="-1.651" y1="0" x2="1.651" y2="0" width="0.254" layer="94"/>
              <text x="0" y="-1.778" size="1.27" layer="96" align="top-center">&gt;VALUE</text>
              <pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
              <wire x1="-1.016" y1="-0.508" x2="1.016" y2="-0.508" width="0.254" layer="94"/>
              <wire x1="-0.381" y1="-1.016" x2="0.381" y2="-1.016" width="0.254" layer="94"/>
            </symbol>
            <symbol name="VCC">
              <wire x1="1.27" y1="0.635" x2="0" y2="2.54" width="0.254" layer="94"/>
              <wire x1="0" y1="2.54" x2="-1.27" y2="0.635" width="0.254" layer="94"/>
              <text x="0" y="3.175" size="1.27" layer="96" align="bottom-center">&gt;VALUE</text>
              <pin name="VCC" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
            </symbol>
            <symbol name="VEE">
              <wire x1="-1.27" y1="-0.635" x2="0" y2="-2.54" width="0.254" layer="94"/>
              <wire x1="0" y1="-2.54" x2="1.27" y2="-0.635" width="0.254" layer="94"/>
              <text x="0" y="-3.175" size="1.27" layer="96" rot="R180" align="bottom-center">&gt;VALUE</text>
              <pin name="VEE" x="0" y="0" visible="off" length="short" direction="sup" rot="R270"/>
            </symbol>
          </symbols>
          <devicesets>
            <deviceset name="SPAD+-" prefix="J">
              <gates>
                <gate name="-" symbol="SPAD" x="0" y="-2.54"/>
                <gate name="+" symbol="SPAD" x="0" y="2.54"/>
              </gates>
              <devices>
                <device name="+-" package="SPAD+-">
                  <connects>
                    <connect gate="+" pin="P$1" pad="2"/>
                    <connect gate="-" pin="P$1" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="-+" package="SPAD-+">
                  <connects>
                    <connect gate="+" pin="P$1" pad="2"/>
                    <connect gate="-" pin="P$1" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="GND" prefix="S">
              <gates>
                <gate name="1" symbol="GND" x="0" y="0"/>
              </gates>
              <devices>
                <device name="">
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="VCC">
              <gates>
                <gate name="G$1" symbol="VCC" x="0" y="0"/>
              </gates>
              <devices>
                <device name="">
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="VEE">
              <gates>
                <gate name="G$1" symbol="VEE" x="0" y="0"/>
              </gates>
              <devices>
                <device name="">
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
          </devicesets>
        </library>
        <library name="r">
          <description>&lt;b&gt;Potentiometers&lt;/b&gt;&lt;p&gt;
 Beckman, Copal, Piher, Spectrol, Schukat&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
          <packages>
            <package name="0202/10">
              <wire x1="-1.27" y1="0.381" x2="-1.016" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="-0.381" x2="-1.016" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-0.635" x2="1.27" y2="-0.381" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="0.635" x2="1.27" y2="0.381" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="0.8"/>
              <pad name="2" x="5.08" y="0" drill="0.8"/>
              <text x="0" y="1.27" size="0.6096" layer="25" ratio="10" rot="R180" align="center">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" align="center">&gt;VALUE</text>
              <wire x1="5.08" y1="0" x2="1.651" y2="0" width="0.254" layer="51"/>
              <wire x1="-5.08" y1="0" x2="-1.651" y2="0" width="0.254" layer="51"/>
              <wire x1="-1.27" y1="0.381" x2="-1.016" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="-0.381" x2="-1.016" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-0.635" x2="1.27" y2="-0.381" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="0.635" x2="1.27" y2="0.381" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="-0.381" x2="-1.27" y2="0.381" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="0.635" x2="-1.016" y2="0.635" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-0.635" x2="-1.016" y2="-0.635" width="0.1524" layer="21"/>
              <wire x1="1.27" y1="-0.381" x2="1.27" y2="0.381" width="0.1524" layer="21"/>
              <rectangle x1="1.27" y1="-0.127" x2="1.651" y2="0.127" layer="21"/>
              <rectangle x1="-1.651" y1="-0.127" x2="-1.27" y2="0.127" layer="21"/>
            </package>
            <package name="0202/10-V">
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="-5.08" drill="0.7" rot="R90"/>
              <pad name="2" x="0" y="5.08" drill="0.7" rot="R90"/>
              <text x="-0.254" y="1.905" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="-0.254" y="-1.905" size="0.6096" layer="27" ratio="10" align="top-right">&gt;VALUE</text>
              <wire x1="0" y1="5.08" x2="0" y2="1.651" width="0.254" layer="51"/>
              <wire x1="0" y1="-5.08" x2="0" y2="-1.651" width="0.254" layer="51"/>
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="-0.381" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="1.016" x2="-0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="1.016" x2="0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.381" y1="1.27" x2="-0.381" y2="1.27" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R90"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R90"/>
            </package>
            <package name="0202/12">
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="-6.35" drill="0.8" rot="R90"/>
              <pad name="2" x="0" y="6.35" drill="0.8" rot="R90"/>
              <text x="-1.27" y="0" size="0.6096" layer="25" ratio="10" rot="R270" align="center">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R90" align="center">&gt;VALUE</text>
              <wire x1="0" y1="6.35" x2="0" y2="1.651" width="0.127" layer="51"/>
              <wire x1="0" y1="-6.35" x2="0" y2="-1.651" width="0.127" layer="51"/>
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="-0.381" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="1.016" x2="-0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="1.016" x2="0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.381" y1="1.27" x2="-0.381" y2="1.27" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R90"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R90"/>
            </package>
            <package name="0202/15">
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="-7.62" drill="0.8" rot="R90"/>
              <pad name="2" x="0" y="7.62" drill="0.8" rot="R90"/>
              <text x="-1.27" y="0" size="0.6096" layer="25" ratio="10" rot="R270" align="center">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R90" align="center">&gt;VALUE</text>
              <wire x1="0" y1="7.62" x2="0" y2="1.651" width="0.127" layer="51"/>
              <wire x1="0" y1="-7.62" x2="0" y2="-1.651" width="0.127" layer="51"/>
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="-0.381" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="1.016" x2="-0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="1.016" x2="0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.381" y1="1.27" x2="-0.381" y2="1.27" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R90"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R90"/>
            </package>
            <package name="0204/10">
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="0.8"/>
              <pad name="2" x="5.08" y="0" drill="0.8"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
              <wire x1="5.08" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-5.08" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
            </package>
            <package name="0204/10-V">
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="5.08" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-5.08" drill="0.8" rot="R270"/>
              <text x="-0.635" y="3.175" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
              <wire x1="0" y1="-5.08" x2="0" y2="-2.921" width="0.508" layer="51"/>
              <wire x1="0" y1="5.08" x2="0" y2="2.921" width="0.508" layer="51"/>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="0.762" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="2.286" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="1.778" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="2.286" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="1.778" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-2.286" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-2.286" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
            </package>
            <package name="0204/7">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0204, grid 7.5 mm</description>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-3.81" y="0" drill="0.8"/>
              <pad name="2" x="3.81" y="0" drill="0.8"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
              <wire x1="3.81" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-3.81" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
            </package>
            <package name="0204/7-V">
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="3.81" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-3.81" drill="0.8" rot="R270"/>
              <text x="-1.27" y="3.2004" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
              <wire x1="0" y1="-3.81" x2="0" y2="-2.921" width="0.508" layer="51"/>
              <wire x1="0" y1="3.81" x2="0" y2="2.921" width="0.508" layer="51"/>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="0.762" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="2.286" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="1.778" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="2.286" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="1.778" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-2.286" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-2.286" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
            </package>
            <package name="0202/7-V">
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="3.81" drill="0.7" rot="R90"/>
              <pad name="2" x="0" y="-3.81" drill="0.7" rot="R90"/>
              <text x="-0.254" y="1.905" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="-0.254" y="-1.905" size="0.6096" layer="27" ratio="10" align="top-right">&gt;VALUE</text>
              <wire x1="0" y1="3.81" x2="0" y2="1.651" width="0.254" layer="51"/>
              <wire x1="0" y1="-3.81" x2="0" y2="-1.651" width="0.254" layer="51"/>
              <wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.381" y1="-1.27" x2="-0.381" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="1.016" x2="-0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="1.016" x2="0.635" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="0.381" y1="1.27" x2="-0.381" y2="1.27" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R90"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R90"/>
            </package>
            <package name="0202/5">
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" align="center">&gt;VALUE</text>
              <wire x1="2.54" y1="0" x2="1.651" y2="0" width="0.254" layer="51"/>
              <wire x1="-2.54" y1="0" x2="-1.651" y2="0" width="0.254" layer="51"/>
              <wire x1="-1.27" y1="-0.381" x2="-1.016" y2="-0.635" width="0.127" layer="21" curve="90"/>
              <wire x1="1.016" y1="-0.635" x2="1.27" y2="-0.381" width="0.127" layer="21" curve="90"/>
              <text x="-1.27" y="0.635" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <wire x1="-1.27" y1="0.381" x2="-1.016" y2="0.635" width="0.127" layer="21" curve="-90"/>
              <wire x1="1.016" y1="0.635" x2="1.27" y2="0.381" width="0.127" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="-0.381" x2="-1.27" y2="0.381" width="0.127" layer="21"/>
              <wire x1="1.016" y1="0.635" x2="-1.016" y2="0.635" width="0.127" layer="21"/>
              <wire x1="1.016" y1="-0.635" x2="-1.016" y2="-0.635" width="0.127" layer="21"/>
              <wire x1="1.27" y1="-0.381" x2="1.27" y2="0.381" width="0.127" layer="21"/>
              <rectangle x1="1.27" y1="-0.127" x2="1.651" y2="0.127" layer="21"/>
              <rectangle x1="-1.651" y1="-0.127" x2="-1.27" y2="0.127" layer="21"/>
            </package>
            <package name="0202/5-V">
              <pad name="1" x="0" y="2.54" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-2.54" drill="0.8" rot="R270"/>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" align="center">&gt;VALUE</text>
              <wire x1="0" y1="-2.54" x2="0" y2="-1.651" width="0.254" layer="51"/>
              <wire x1="0" y1="2.54" x2="0" y2="1.651" width="0.254" layer="51"/>
              <wire x1="-0.381" y1="1.27" x2="-0.635" y2="1.016" width="0.127" layer="21" curve="90"/>
              <wire x1="-0.635" y1="-1.016" x2="-0.381" y2="-1.27" width="0.127" layer="21" curve="90"/>
              <text x="-0.5715" y="1.143" size="0.6096" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <wire x1="0.381" y1="1.27" x2="0.635" y2="1.016" width="0.127" layer="21" curve="-90"/>
              <wire x1="0.635" y1="-1.016" x2="0.381" y2="-1.27" width="0.127" layer="21" curve="-90"/>
              <wire x1="-0.381" y1="1.27" x2="0.381" y2="1.27" width="0.127" layer="21"/>
              <wire x1="0.635" y1="-1.016" x2="0.635" y2="1.016" width="0.127" layer="21"/>
              <wire x1="-0.635" y1="-1.016" x2="-0.635" y2="1.016" width="0.127" layer="21"/>
              <wire x1="-0.381" y1="-1.27" x2="0.381" y2="-1.27" width="0.127" layer="21"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R270"/>
            </package>
            <package name="0202/7">
              <wire x1="0.381" y1="1.27" x2="0.635" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.381" y1="1.27" x2="-0.635" y2="1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="-1.016" x2="-0.381" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="-1.016" x2="0.381" y2="-1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="3.81" drill="0.7" rot="R270"/>
              <pad name="2" x="0" y="-3.81" drill="0.7" rot="R270"/>
              <text x="-0.889" y="0" size="0.6096" layer="25" ratio="10" rot="R270" align="top-center">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <wire x1="0" y1="-3.81" x2="0" y2="-1.651" width="0.254" layer="51"/>
              <wire x1="0" y1="3.81" x2="0" y2="1.651" width="0.254" layer="51"/>
              <wire x1="0.381" y1="1.27" x2="0.635" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.381" y1="1.27" x2="-0.635" y2="1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.635" y1="-1.016" x2="-0.381" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.635" y1="-1.016" x2="0.381" y2="-1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.381" y1="1.27" x2="0.381" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="-1.016" x2="0.635" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="-1.016" x2="-0.635" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="-1.27" x2="0.381" y2="-1.27" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="-1.5875" x2="0.1905" y2="-1.3335" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="1.3335" x2="0.1905" y2="1.5875" layer="21" rot="R270"/>
            </package>
            <package name="0204/12">
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-6.35" y="0" drill="0.8"/>
              <pad name="2" x="6.35" y="0" drill="0.8"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
              <wire x1="6.35" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-6.35" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
            </package>
            <package name="0204/12-V">
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="6.35" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-6.35" drill="0.8" rot="R270"/>
              <text x="-1.2954" y="-2.54" size="0.6096" layer="25" ratio="10" rot="R90">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
              <wire x1="0" y1="-6.35" x2="0" y2="-2.921" width="0.508" layer="51"/>
              <wire x1="0" y1="6.35" x2="0" y2="2.921" width="0.508" layer="51"/>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="0.762" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="2.286" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="1.778" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="2.286" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="1.778" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-2.286" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-2.286" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.1524" layer="21"/>
              <text x="-1.2954" y="-2.54" size="0.6096" layer="25" ratio="10" rot="R90">&gt;NAME</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
            </package>
            <package name="0204/15">
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.62" y="0" drill="0.8"/>
              <pad name="2" x="7.62" y="0" drill="0.8"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
              <wire x1="7.62" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-7.62" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
            </package>
            <package name="0204/15-V">
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="7.62" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-7.62" drill="0.8" rot="R270"/>
              <text x="-1.2954" y="-2.54" size="0.6096" layer="25" ratio="10" rot="R90">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
              <wire x1="0" y1="-7.62" x2="0" y2="-2.921" width="0.508" layer="51"/>
              <wire x1="0" y1="7.62" x2="0" y2="2.921" width="0.508" layer="51"/>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="0.762" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="2.286" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="1.778" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="2.286" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="1.778" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-2.286" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-2.286" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.1524" layer="21"/>
              <text x="-1.2954" y="-2.54" size="0.6096" layer="25" ratio="10" rot="R90">&gt;NAME</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
            </package>
            <package name="0204/2V">
              <circle x="0" y="-1.27" radius="0.889" width="0.1524" layer="51"/>
              <circle x="0" y="-1.27" radius="0.635" width="0.0508" layer="51"/>
              <pad name="2" x="0" y="-1.27" drill="0.8" rot="R90"/>
              <pad name="1" x="0" y="1.27" drill="0.8" rot="R90"/>
              <text x="0" y="-2.54" size="0.6096" layer="27" ratio="10" align="top-center">&gt;VALUE</text>
              <wire x1="0" y1="-1.27" x2="0" y2="1.27" width="0.508" layer="51"/>
              <wire x1="0" y1="-0.127" x2="0" y2="0.127" width="0.508" layer="21"/>
              <circle x="0" y="-1.27" radius="0.889" width="0.1524" layer="51"/>
              <circle x="0" y="-1.27" radius="0.635" width="0.0508" layer="51"/>
              <text x="0" y="2.286" size="0.6096" layer="25" ratio="10" align="bottom-center">&gt;NAME</text>
            </package>
            <package name="0204/2">
              <circle x="1.27" y="0" radius="0.889" width="0.1524" layer="51"/>
              <circle x="1.27" y="0" radius="0.635" width="0.0508" layer="51"/>
              <pad name="1" x="1.27" y="0" drill="0.8" rot="R180"/>
              <pad name="2" x="-1.27" y="0" drill="0.8" rot="R180"/>
              <text x="-2.1336" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <wire x1="1.27" y1="0" x2="-1.27" y2="0" width="0.508" layer="51"/>
              <wire x1="0.127" y1="0" x2="-0.127" y2="0" width="0.508" layer="21"/>
              <circle x="1.27" y="0" radius="0.889" width="0.1524" layer="51"/>
              <circle x="1.27" y="0" radius="0.635" width="0.0508" layer="51"/>
              <text x="-2.1336" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.1336" y="-1.0414" size="0.6096" layer="27" ratio="10" align="top-left">&gt;VALUE</text>
            </package>
            <package name="0204V">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0204, grid 2.5 mm</description>
              <circle x="-1.27" y="0" radius="0.889" width="0.1524" layer="51"/>
              <circle x="-1.27" y="0" radius="0.635" width="0.0508" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.1336" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.1336" y="-2.3114" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.508" layer="51"/>
              <wire x1="-0.127" y1="0" x2="0.127" y2="0" width="0.508" layer="21"/>
              <circle x="-1.27" y="0" radius="0.889" width="0.1524" layer="51"/>
              <circle x="-1.27" y="0" radius="0.635" width="0.0508" layer="51"/>
              <text x="-2.1336" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.1336" y="-2.3114" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
            </package>
            <package name="POT">
              <wire x1="-4.54" y1="-2.43" x2="4.54" y2="-2.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-4.43" x2="-3.44" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-3.44" y1="-4.43" x2="-3.44" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-1.7" x2="4.54" y2="-1.7" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="-0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-9.89" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-2.53" x2="-1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-6.09" x2="-0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-6.39" x2="1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="1.495" y1="-6.09" x2="1.495" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-9.89" x2="0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="1.504" width="0.2032" layer="21"/>
              <wire x1="4.54" y1="-2.43" x2="4.54" y2="1.504" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.755" x2="1.395" y2="-5.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.255" x2="1.395" y2="-4.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.755" x2="1.395" y2="-4.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.255" x2="1.395" y2="-3.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.755" x2="1.395" y2="-3.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.255" x2="1.395" y2="-2.73" width="0.2032" layer="21"/>
              <wire x1="-1.17" y1="-6.215" x2="1.395" y2="-5.69" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="1.504" x2="4.54" y2="1.504" width="0.2032" layer="21"/>
              <pad name="1" x="-2.54" y="0" drill="1" diameter="1.9304"/>
              <pad name="2" x="0" y="0" drill="1" diameter="1.9304"/>
              <pad name="3" x="2.54" y="0" drill="1" diameter="1.9304"/>
              <text x="-4.572" y="1.778" size="0.6096" layer="25">&gt;NAME</text>
              <text x="4.572" y="1.778" size="0.6096" layer="27" align="bottom-right">&gt;VALUE</text>
            </package>
            <package name="POT-V">
              <wire x1="-0.254" y1="0.254" x2="-0.254" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-0.254" y1="0.889" x2="0.254" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0.889" x2="0.254" y2="0.254" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0.254" x2="0.889" y2="0.254" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="0.254" x2="0.889" y2="-0.254" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-0.254" x2="0.254" y2="-0.254" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="-0.254" x2="0.254" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="-0.889" x2="-0.254" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="-0.254" y1="-0.889" x2="-0.254" y2="-0.254" width="0.1524" layer="21"/>
              <wire x1="-0.254" y1="-0.254" x2="-0.889" y2="-0.254" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-0.254" x2="-0.889" y2="0.254" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="0.254" x2="-0.254" y2="0.254" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="3.81" x2="-0.889" y2="3.81" width="0.1524" layer="51"/>
              <wire x1="3.429" y1="3.81" x2="0.889" y2="3.81" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-0.889" x2="-1.397" y2="-1.397" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-0.889" x2="1.397" y2="-1.397" width="0.1524" layer="21"/>
              <wire x1="-1.905" y1="0" x2="-1.397" y2="0" width="0.1524" layer="21"/>
              <wire x1="1.397" y1="0" x2="1.905" y2="0" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="1.016" x2="-1.27" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="1.016" x2="1.27" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="0" y1="1.397" x2="0" y2="1.905" width="0.1524" layer="51"/>
              <wire x1="3.429" y1="-3.81" x2="1.651" y2="-3.81" width="0.1524" layer="51"/>
              <wire x1="1.651" y1="-3.81" x2="-1.651" y2="-3.81" width="0.1524" layer="21"/>
              <wire x1="-1.651" y1="-3.81" x2="-3.429" y2="-3.81" width="0.1524" layer="51"/>
              <wire x1="3.429" y1="-3.81" x2="3.81" y2="-3.429" width="0.1524" layer="21" curve="90"/>
              <wire x1="3.429" y1="3.81" x2="3.81" y2="3.429" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.81" y1="3.429" x2="-3.429" y2="3.81" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.81" y1="-3.429" x2="-3.429" y2="-3.81" width="0.1524" layer="21" curve="90"/>
              <wire x1="3.81" y1="-1.524" x2="3.81" y2="-3.429" width="0.1524" layer="51"/>
              <wire x1="-3.429" y1="3.81" x2="-0.889" y2="3.81" width="0.1524" layer="21"/>
              <wire x1="-3.81" y1="-3.429" x2="-3.81" y2="-1.524" width="0.1524" layer="51"/>
              <wire x1="3.81" y1="-1.524" x2="3.81" y2="3.429" width="0.1524" layer="21"/>
              <wire x1="-3.81" y1="3.429" x2="-3.81" y2="-1.524" width="0.1524" layer="21"/>
              <circle x="0" y="0" radius="2.032" width="0.1524" layer="51"/>
              <circle x="0" y="0" radius="1.27" width="0.1524" layer="51"/>
              <pad name="S" x="0" y="2.54" drill="1.016" diameter="1.9304"/>
              <pad name="A" x="-2.54" y="-2.54" drill="1.016" diameter="1.9304" rot="R90"/>
              <pad name="E" x="2.54" y="-2.54" drill="1.016" diameter="1.9304" rot="R90"/>
              <text x="-2.54" y="4.191" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-3.81" y="-5.461" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="1.905" y1="-3.81" x2="3.175" y2="-3.048" layer="51"/>
            </package>
            <package name="POT-P5">
              <wire x1="-4.54" y1="-2.43" x2="4.54" y2="-2.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-4.43" x2="-3.44" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-3.44" y1="-4.43" x2="-3.44" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-1.7" x2="4.54" y2="-1.7" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="-0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-9.89" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-2.53" x2="-1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-6.09" x2="-0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-6.39" x2="1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="1.495" y1="-6.09" x2="1.495" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-9.89" x2="0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="0.742" width="0.2032" layer="21"/>
              <wire x1="4.54" y1="-2.43" x2="4.54" y2="0.742" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.755" x2="1.395" y2="-5.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.255" x2="1.395" y2="-4.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.755" x2="1.395" y2="-4.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.255" x2="1.395" y2="-3.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.755" x2="1.395" y2="-3.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.255" x2="1.395" y2="-2.73" width="0.2032" layer="21"/>
              <wire x1="-1.17" y1="-6.215" x2="1.395" y2="-5.69" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="0.742" x2="4.54" y2="0.742" width="0.2032" layer="21"/>
              <pad name="A" x="-5.08" y="2.54" drill="1" diameter="1.9304"/>
              <pad name="S" x="0" y="2.54" drill="1" diameter="1.9304"/>
              <pad name="E" x="5.08" y="2.54" drill="1" diameter="1.9304"/>
              <text x="0" y="-0.508" size="0.6096" layer="25" align="center">&gt;NAME</text>
              <text x="2.032" y="-2.921" size="0.6096" layer="27" align="top-left">&gt;VALUE</text>
              <wire x1="-5.08" y1="2.54" x2="-3.81" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="-3.81" y1="2.54" x2="-2.54" y2="0.762" width="0.4064" layer="21"/>
              <wire x1="0" y1="0.762" x2="0" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="5.08" y1="2.54" x2="3.81" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="3.81" y1="2.54" x2="2.54" y2="0.762" width="0.4064" layer="21"/>
            </package>
            <package name="CONN-3P">
              <pad name="3" x="0" y="2.54" drill="0.9" diameter="1.778" shape="octagon" rot="R270"/>
              <pad name="2" x="0" y="0" drill="0.9" diameter="1.778" shape="octagon" rot="R270"/>
              <pad name="1" x="0" y="-2.54" drill="0.9" diameter="1.778" shape="square" rot="R270"/>
              <wire x1="-1.27" y1="-3.81" x2="1.27" y2="-3.81" width="0.127" layer="21"/>
              <wire x1="1.27" y1="-3.81" x2="1.27" y2="3.81" width="0.127" layer="21"/>
              <wire x1="1.27" y1="3.81" x2="-1.27" y2="3.81" width="0.127" layer="21"/>
              <wire x1="-1.27" y1="3.81" x2="-1.27" y2="-3.81" width="0.127" layer="21"/>
              <text x="-2.54" y="-3.556" size="0.6096" layer="23" rot="R270" align="bottom-right">1</text>
              <text x="0" y="4.445" size="1.016" layer="25" align="bottom-center">&gt;NAME</text>
              <text x="0" y="-4.445" size="0.6096" layer="27" align="top-center">&gt;VALUE</text>
            </package>
            <package name="POT-P5-2-5">
              <wire x1="-4.54" y1="-2.43" x2="4.54" y2="-2.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-4.43" x2="-3.44" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="-4.43" width="0.2032" layer="21"/>
              <wire x1="-3.44" y1="-4.43" x2="-3.44" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-1.7" x2="4.54" y2="-1.7" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="-0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-9.89" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-2.53" x2="-1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="-1.495" y1="-6.09" x2="-0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="0.995" y1="-6.39" x2="1.495" y2="-6.09" width="0.2032" layer="21"/>
              <wire x1="1.495" y1="-6.09" x2="1.495" y2="-2.53" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-6.39" x2="0.995" y2="-6.39" width="0.2032" layer="21"/>
              <wire x1="-0.995" y1="-9.89" x2="0.995" y2="-9.89" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="-2.43" x2="-4.54" y2="0.742" width="0.2032" layer="21"/>
              <wire x1="4.54" y1="-2.43" x2="4.54" y2="0.742" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.755" x2="1.395" y2="-5.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-5.255" x2="1.395" y2="-4.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.755" x2="1.395" y2="-4.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-4.255" x2="1.395" y2="-3.73" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.755" x2="1.395" y2="-3.23" width="0.2032" layer="21"/>
              <wire x1="-1.395" y1="-3.255" x2="1.395" y2="-2.73" width="0.2032" layer="21"/>
              <wire x1="-1.17" y1="-6.215" x2="1.395" y2="-5.69" width="0.2032" layer="21"/>
              <wire x1="-4.54" y1="0.742" x2="4.54" y2="0.742" width="0.2032" layer="21"/>
              <pad name="A" x="-5.08" y="2.54" drill="1" diameter="1.9304"/>
              <pad name="S" x="0" y="2.54" drill="1" diameter="1.9304"/>
              <pad name="E" x="5.08" y="2.54" drill="1" diameter="1.9304"/>
              <text x="0" y="-0.508" size="0.6096" layer="25" align="center">&gt;NAME</text>
              <text x="2.032" y="-2.921" size="0.6096" layer="27" align="top-left">&gt;VALUE</text>
              <wire x1="-5.08" y1="2.54" x2="-3.81" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="-3.81" y1="2.54" x2="-2.54" y2="0.762" width="0.4064" layer="21"/>
              <wire x1="0" y1="0.762" x2="0" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="5.08" y1="2.54" x2="3.81" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="3.81" y1="2.54" x2="2.54" y2="0.762" width="0.4064" layer="21"/>
              <pad name="A1" x="-5.08" y="7.62" drill="1" diameter="1.9304"/>
              <pad name="S1" x="0" y="7.62" drill="1" diameter="1.9304"/>
              <pad name="E1" x="5.08" y="7.62" drill="1" diameter="1.9304"/>
              <wire x1="-5.08" y1="7.62" x2="-5.08" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="0" y1="7.62" x2="0" y2="2.54" width="0.4064" layer="21"/>
              <wire x1="5.08" y1="7.62" x2="5.08" y2="2.54" width="0.4064" layer="21"/>
            </package>
            <package name="0204/5">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0204, grid 5 mm</description>
              <wire x1="-1.778" y1="0.635" x2="-1.524" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.778" y1="-0.635" x2="-1.524" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.524" y1="-0.889" x2="1.778" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.524" y1="0.889" x2="1.778" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-2.0066" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.1336" y="-2.3114" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="-2.032" y1="-0.254" x2="-1.778" y2="0.254" layer="51"/>
              <rectangle x1="1.778" y1="-0.254" x2="2.032" y2="0.254" layer="51"/>
              <wire x1="2.54" y1="0" x2="2.032" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0" x2="-2.032" y2="0" width="0.508" layer="51"/>
              <wire x1="-1.778" y1="0.635" x2="-1.524" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.778" y1="-0.635" x2="-1.524" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.524" y1="-0.889" x2="1.778" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.524" y1="0.889" x2="1.778" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.778" y1="-0.635" x2="-1.778" y2="0.635" width="0.1524" layer="51"/>
              <wire x1="-1.524" y1="0.889" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-1.143" y1="0.762" x2="-1.27" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-1.524" y1="-0.889" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="-1.143" y1="-0.762" x2="-1.27" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="1.143" y1="0.762" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.143" y1="0.762" x2="-1.143" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="1.143" y1="-0.762" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="1.143" y1="-0.762" x2="-1.143" y2="-0.762" width="0.1524" layer="21"/>
              <wire x1="1.524" y1="0.889" x2="1.27" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.524" y1="-0.889" x2="1.27" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.635" x2="1.778" y2="0.635" width="0.1524" layer="51"/>
              <text x="-2.0066" y="1.1684" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.1336" y="-2.3114" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="-2.032" y1="-0.254" x2="-1.778" y2="0.254" layer="51"/>
              <rectangle x1="1.778" y1="-0.254" x2="2.032" y2="0.254" layer="51"/>
            </package>
            <package name="0207/10">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 10 mm</description>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="0.8"/>
              <pad name="2" x="5.08" y="0" drill="0.8"/>
              <text x="-3.048" y="1.524" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.2606" y="-0.635" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
              <wire x1="5.08" y1="0" x2="4.064" y2="0" width="0.6096" layer="51"/>
              <wire x1="-5.08" y1="0" x2="-4.064" y2="0" width="0.6096" layer="51"/>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
              <text x="-3.048" y="1.524" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.2606" y="-0.635" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
            </package>
            <package name="0207/12">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 12 mm</description>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-6.35" y="0" drill="0.8"/>
              <pad name="2" x="6.35" y="0" drill="0.8"/>
              <text x="-3.175" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.6858" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
              <rectangle x1="4.445" y1="-0.3048" x2="5.3086" y2="0.3048" layer="21"/>
              <rectangle x1="-5.3086" y1="-0.3048" x2="-4.445" y2="0.3048" layer="21"/>
              <wire x1="6.35" y1="0" x2="5.334" y2="0" width="0.6096" layer="51"/>
              <wire x1="-6.35" y1="0" x2="-5.334" y2="0" width="0.6096" layer="51"/>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="4.445" y1="0" x2="4.064" y2="0" width="0.6096" layer="21"/>
              <wire x1="-4.445" y1="0" x2="-4.064" y2="0" width="0.6096" layer="21"/>
              <text x="-3.175" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.6858" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
              <rectangle x1="4.445" y1="-0.3048" x2="5.3086" y2="0.3048" layer="21"/>
              <rectangle x1="-5.3086" y1="-0.3048" x2="-4.445" y2="0.3048" layer="21"/>
            </package>
            <package name="0207/15">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 15mm</description>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.62" y="0" drill="0.8"/>
              <pad name="2" x="7.62" y="0" drill="0.8"/>
              <text x="-3.175" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.6858" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
              <rectangle x1="5.715" y1="-0.3048" x2="6.5786" y2="0.3048" layer="21"/>
              <rectangle x1="-6.5786" y1="-0.3048" x2="-5.715" y2="0.3048" layer="21"/>
              <wire x1="7.62" y1="0" x2="6.604" y2="0" width="0.6096" layer="51"/>
              <wire x1="-7.62" y1="0" x2="-6.604" y2="0" width="0.6096" layer="51"/>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="5.715" y1="0" x2="4.064" y2="0" width="0.6096" layer="21"/>
              <wire x1="-5.715" y1="0" x2="-4.064" y2="0" width="0.6096" layer="21"/>
              <text x="-3.175" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.6858" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
              <rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
              <rectangle x1="5.715" y1="-0.3048" x2="6.5786" y2="0.3048" layer="21"/>
              <rectangle x1="-6.5786" y1="-0.3048" x2="-5.715" y2="0.3048" layer="21"/>
            </package>
            <package name="0207/2V">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 2.5 mm</description>
              <circle x="-1.27" y="0" radius="1.27" width="0.1524" layer="21"/>
              <circle x="-1.27" y="0" radius="1.016" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-0.0508" y="1.016" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-0.0508" y="-2.2352" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <wire x1="-1.27" y1="0" x2="-0.381" y2="0" width="0.6096" layer="51"/>
              <wire x1="-0.254" y1="0" x2="0.254" y2="0" width="0.6096" layer="21"/>
              <wire x1="0.381" y1="0" x2="1.27" y2="0" width="0.6096" layer="51"/>
              <circle x="-1.27" y="0" radius="1.27" width="0.1524" layer="21"/>
              <circle x="-1.27" y="0" radius="1.016" width="0.1524" layer="51"/>
              <text x="-0.0508" y="1.016" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-0.0508" y="-2.2352" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
            </package>
            <package name="0207/5V">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 5 mm</description>
              <circle x="-2.54" y="0" radius="1.27" width="0.1016" layer="21"/>
              <circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-1.143" y="0.889" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.143" y="-2.159" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <wire x1="-2.54" y1="0" x2="-0.889" y2="0" width="0.6096" layer="51"/>
              <wire x1="-0.762" y1="0" x2="0.762" y2="0" width="0.6096" layer="21"/>
              <wire x1="0.889" y1="0" x2="2.54" y2="0" width="0.6096" layer="51"/>
              <circle x="-2.54" y="0" radius="1.27" width="0.1016" layer="21"/>
              <circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
              <text x="-1.143" y="0.889" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-1.143" y="-2.159" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
            </package>
            <package name="0207/7">
              <description>&lt;b&gt;RESISTOR&lt;/b&gt;&lt;p&gt;
 type 0207, grid 7.5 mm</description>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-3.81" y="0" drill="0.8"/>
              <pad name="2" x="3.81" y="0" drill="0.8"/>
              <text x="-2.54" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.5588" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="-3.429" y1="-0.3048" x2="-3.175" y2="0.3048" layer="51"/>
              <rectangle x1="3.175" y1="-0.3048" x2="3.429" y2="0.3048" layer="51"/>
              <wire x1="-3.81" y1="0" x2="-3.429" y2="0" width="0.6096" layer="51"/>
              <wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="51"/>
              <wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="51"/>
              <wire x1="3.429" y1="0" x2="3.81" y2="0" width="0.6096" layer="51"/>
              <text x="-2.54" y="1.397" size="0.6096" layer="25" ratio="10">&gt;NAME</text>
              <text x="-2.286" y="-0.5588" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <rectangle x1="-3.429" y1="-0.3048" x2="-3.175" y2="0.3048" layer="51"/>
              <rectangle x1="3.175" y1="-0.3048" x2="3.429" y2="0.3048" layer="51"/>
            </package>
            <package name="0202/2V">
              <pad name="1" x="0" y="-1.27" drill="0.8" rot="R90"/>
              <pad name="2" x="0" y="1.27" drill="0.8" rot="R90"/>
              <text x="-0.254" y="0.508" size="0.6096" layer="25" ratio="10" rot="R180">&gt;NAME</text>
              <text x="0.254" y="-0.508" size="0.6096" layer="27" ratio="10">&gt;VALUE</text>
              <circle x="0" y="-0.762" radius="0.762" width="0.127" layer="21"/>
              <wire x1="0" y1="-0.762" x2="0" y2="1.27" width="0.254" layer="51"/>
              <wire x1="0" y1="-0.127" x2="0" y2="0.127" width="0.254" layer="21"/>
              <circle x="0" y="-0.762" radius="0.5588" width="0.1524" layer="51"/>
              <circle x="0" y="-0.762" radius="0.6858" width="0.0508" layer="51"/>
            </package>
            <package name="0202V">
              <pad name="2" x="0" y="1.27" drill="0.8" rot="R270"/>
              <pad name="1" x="0" y="-1.27" drill="0.8" rot="R270"/>
              <text x="-1.016" y="0.508" size="0.6096" layer="25" ratio="10" align="center-right">&gt;NAME</text>
              <text x="-1.016" y="-0.508" size="0.6096" layer="27" ratio="10" align="center-right">&gt;VALUE</text>
              <circle x="0" y="0.762" radius="0.762" width="0.127" layer="21"/>
              <wire x1="0" y1="0.762" x2="0" y2="-1.27" width="0.254" layer="51"/>
              <wire x1="0" y1="0.127" x2="0" y2="-0.127" width="0.254" layer="21"/>
              <circle x="0" y="0.762" radius="0.508" width="0.0508" layer="51"/>
            </package>
          </packages>
          <symbols>
            <symbol name="R-H">
              <text x="0" y="1.4986" size="1.27" layer="95" align="bottom-center">&gt;NAME</text>
              <pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
              <pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
              <wire x1="-2.54" y1="-0.889" x2="2.54" y2="-0.889" width="0.254" layer="94"/>
              <wire x1="2.54" y1="0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
              <wire x1="2.54" y1="-0.889" x2="2.54" y2="0.889" width="0.254" layer="94"/>
              <wire x1="-2.54" y1="-0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
              <text x="0" y="0" size="1.27" layer="96" rot="R180" align="center">&gt;VALUE</text>
            </symbol>
            <symbol name="POT">
              <wire x1="-0.762" y1="2.54" x2="-0.762" y2="-2.54" width="0.254" layer="94"/>
              <wire x1="0.762" y1="-2.54" x2="0.762" y2="2.54" width="0.254" layer="94"/>
              <wire x1="2.54" y1="0" x2="1.651" y2="0" width="0.1524" layer="94"/>
              <wire x1="0.762" y1="2.54" x2="-0.762" y2="2.54" width="0.254" layer="94"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.254" layer="94"/>
              <wire x1="-2.54" y1="-2.54" x2="-2.54" y2="-0.508" width="0.1524" layer="94"/>
              <wire x1="-2.54" y1="-0.508" x2="-3.048" y2="-1.524" width="0.1524" layer="94"/>
              <wire x1="-2.54" y1="-0.508" x2="-2.032" y2="-1.524" width="0.1524" layer="94"/>
              <text x="2.54" y="2.54" size="1.27" layer="95" align="top-left">&gt;NAME</text>
              <text x="2.54" y="-2.54" size="1.27" layer="96">&gt;VALUE</text>
              <pin name="A" x="0" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
              <pin name="E" x="0" y="5.08" visible="off" length="short" direction="pas" rot="R270"/>
              <pin name="S" x="2.54" y="0" visible="off" length="point" direction="pas" rot="R180"/>
              <wire x1="1.651" y1="0" x2="-1.8796" y2="1.7526" width="0.1524" layer="94"/>
              <wire x1="-2.1597" y1="1.2939" x2="-3.1989" y2="2.4495" width="0.1524" layer="94"/>
              <wire x1="-3.1989" y1="2.4495" x2="-1.7018" y2="2.2352" width="0.1524" layer="94"/>
              <wire x1="-2.1597" y1="1.2939" x2="-1.7018" y2="2.2352" width="0.1524" layer="94"/>
              <text x="-2.54" y="5.08" size="1.27" layer="97" font="fixed" align="center">A</text>
              <text x="-2.54" y="-5.08" size="1.27" layer="97" font="fixed" align="center">E</text>
            </symbol>
            <symbol name="R">
              <pin name="1" x="0" y="5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R270"/>
              <pin name="2" x="0" y="-5.08" visible="off" length="short" direction="pas" swaplevel="1" rot="R90"/>
              <wire x1="0.889" y1="-2.54" x2="0.889" y2="2.54" width="0.254" layer="94"/>
              <wire x1="-0.889" y1="2.54" x2="-0.889" y2="-2.54" width="0.254" layer="94"/>
              <wire x1="0.889" y1="2.54" x2="-0.889" y2="2.54" width="0.254" layer="94"/>
              <wire x1="0.889" y1="-2.54" x2="-0.889" y2="-2.54" width="0.254" layer="94"/>
              <text x="-1.27" y="1.27" size="1.27" layer="95" align="bottom-right">&gt;NAME</text>
              <text x="-1.27" y="-1.27" size="1.27" layer="96" align="top-right">&gt;VALUE</text>
            </symbol>
          </symbols>
          <devicesets>
            <deviceset name="R-H" prefix="R" uservalue="yes">
              <gates>
                <gate name="G$1" symbol="R-H" x="0" y="0"/>
              </gates>
              <devices>
                <device name="0202/10" package="0202/10">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/10-V" package="0202/10-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/12" package="0202/12">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/15" package="0202/15">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10" package="0204/10">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10-V" package="0204/10-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/7" package="0204/7">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/7-V" package="0204/7-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/7-V" package="0202/7-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/5" package="0202/5">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/5-V" package="0202/5-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/7" package="0202/7">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/12" package="0204/12">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/12-V" package="0204/12-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/15" package="0204/15">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/15-V" package="0204/15-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/2V" package="0204/2V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/2" package="0204/2">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204V" package="0204V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="POT" prefix="R" uservalue="yes">
              <gates>
                <gate name="1" symbol="POT" x="0" y="0"/>
              </gates>
              <devices>
                <device name="" package="POT">
                  <connects>
                    <connect gate="1" pin="A" pad="1"/>
                    <connect gate="1" pin="E" pad="3"/>
                    <connect gate="1" pin="S" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="-V" package="POT-V">
                  <connects>
                    <connect gate="1" pin="A" pad="A"/>
                    <connect gate="1" pin="E" pad="E"/>
                    <connect gate="1" pin="S" pad="S"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="-P5" package="POT-P5">
                  <connects>
                    <connect gate="1" pin="A" pad="A"/>
                    <connect gate="1" pin="E" pad="E"/>
                    <connect gate="1" pin="S" pad="S"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="-3P" package="CONN-3P">
                  <connects>
                    <connect gate="1" pin="A" pad="1"/>
                    <connect gate="1" pin="E" pad="3"/>
                    <connect gate="1" pin="S" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="P5-2-5" package="POT-P5-2-5">
                  <connects>
                    <connect gate="1" pin="A" pad="A A1"/>
                    <connect gate="1" pin="E" pad="E E1"/>
                    <connect gate="1" pin="S" pad="S S1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="R" prefix="R" uservalue="yes">
              <description>&lt;B&gt;RESISTOR&lt;/B&gt;, European symbol</description>
              <gates>
                <gate name="G$1" symbol="R" x="0" y="0"/>
              </gates>
              <devices>
                <device name="0204/5" package="0204/5">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/7" package="0204/7">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204V" package="0204V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/10" package="0207/10">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/12" package="0207/12">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/15" package="0207/15">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/2V" package="0207/2V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/5V" package="0207/5V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0207/7" package="0207/7">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10" package="0204/10">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/12" package="0204/12">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/15" package="0204/15">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/2V" package="0204/2V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/2V" package="0202/2V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/5" package="0202/5">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/7" package="0202/7">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/10" package="0202/10">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202V" package="0202V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/12" package="0202/12">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/15" package="0202/15">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/7-V" package="0202/7-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/5-V" package="0202/5-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0202/10-V" package="0202/10-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/7-V" package="0204/7-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10-V" package="0204/10-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/12-V" package="0204/12-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="2"/>
                    <connect gate="G$1" pin="2" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/15-V" package="0204/15-V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/2" package="0204/2">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
          </devicesets>
        </library>
        <library name="ic">
          <description>&lt;b&gt;CMOS Logic Devices, 4000 Series&lt;/b&gt;&lt;p&gt;
 Based on the following sources:
 &lt;ul&gt;
 &lt;li&gt;Motorola &lt;i&gt;CMOS LOGIC DATA&lt;/i&gt;; book, 02/88, DL131 REV 1
 &lt;li&gt;http://www.elexp.com
 &lt;li&gt;http://www.intersil.com
 &lt;li&gt;http://www.ls3c.com.tw/product/1/COMOS.html
 &lt;/ul&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
          <packages>
            <package name="DIL08">
              <description>&lt;b&gt;Dual In Line Package&lt;/b&gt;</description>
              <wire x1="5.08" y1="2.921" x2="-5.08" y2="2.921" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="-2.921" x2="5.08" y2="-2.921" width="0.1524" layer="21"/>
              <wire x1="5.08" y1="2.921" x2="5.08" y2="-2.921" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="2.921" x2="-5.08" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="-2.921" x2="-5.08" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="1.016" x2="-5.08" y2="-1.016" width="0.1524" layer="21" curve="-180"/>
              <pad name="1" x="-3.81" y="-3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="2" x="-1.27" y="-3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="7" x="-1.27" y="3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="8" x="-3.81" y="3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="3" x="1.27" y="-3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="4" x="3.81" y="-3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="6" x="1.27" y="3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <pad name="5" x="3.81" y="3.81" drill="0.8" diameter="1.27" rot="R90"/>
              <text x="-5.334" y="-2.921" size="1.016" layer="25" ratio="10" rot="R90">&gt;NAME</text>
              <text x="-3.556" y="-0.635" size="0.8128" layer="27" ratio="10">&gt;VALUE</text>
            </package>
            <package name="SO08">
              <description>&lt;b&gt;Small Outline Package 8&lt;/b&gt;&lt;br&gt;
 NS Package M08A</description>
              <wire x1="2.4" y1="1.9" x2="2.4" y2="-1.4" width="0.2032" layer="51"/>
              <wire x1="2.4" y1="-1.4" x2="2.4" y2="-1.9" width="0.2032" layer="51"/>
              <wire x1="2.4" y1="-1.9" x2="-2.4" y2="-1.9" width="0.2032" layer="51"/>
              <wire x1="-2.4" y1="-1.9" x2="-2.4" y2="-1.4" width="0.2032" layer="51"/>
              <wire x1="-2.4" y1="-1.4" x2="-2.4" y2="1.9" width="0.2032" layer="51"/>
              <wire x1="-2.4" y1="1.9" x2="2.4" y2="1.9" width="0.2032" layer="51"/>
              <wire x1="2.4" y1="-1.4" x2="-2.4" y2="-1.4" width="0.2032" layer="51"/>
              <smd name="2" x="-0.635" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="7" x="-0.635" y="2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="1" x="-1.905" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="3" x="0.635" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="4" x="1.905" y="-2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="8" x="-1.905" y="2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="6" x="0.635" y="2.6" dx="0.6" dy="2.2" layer="1"/>
              <smd name="5" x="1.905" y="2.6" dx="0.6" dy="2.2" layer="1"/>
              <text x="-2.667" y="-1.905" size="1.016" layer="25" rot="R90">&gt;NAME</text>
              <text x="3.937" y="-1.905" size="0.8128" layer="27" rot="R90">&gt;VALUE</text>
              <rectangle x1="-2.15" y1="-3.1" x2="-1.66" y2="-2" layer="51"/>
              <rectangle x1="-0.88" y1="-3.1" x2="-0.39" y2="-2" layer="51"/>
              <rectangle x1="0.39" y1="-3.1" x2="0.88" y2="-2" layer="51"/>
              <rectangle x1="1.66" y1="-3.1" x2="2.15" y2="-2" layer="51"/>
              <rectangle x1="1.66" y1="2" x2="2.15" y2="3.1" layer="51"/>
              <rectangle x1="0.39" y1="2" x2="0.88" y2="3.1" layer="51"/>
              <rectangle x1="-0.88" y1="2" x2="-0.39" y2="3.1" layer="51"/>
              <rectangle x1="-2.15" y1="2" x2="-1.66" y2="3.1" layer="51"/>
            </package>
            <package name="DIL16">
              <wire x1="2.921" y1="-10.16" x2="2.921" y2="10.16" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="10.16" x2="-2.921" y2="-10.16" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="-10.16" x2="-2.921" y2="-10.16" width="0.1524" layer="21"/>
              <wire x1="2.921" y1="10.16" x2="1.016" y2="10.16" width="0.1524" layer="21"/>
              <wire x1="-2.921" y1="10.16" x2="-1.016" y2="10.16" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="10.16" x2="-1.016" y2="10.16" width="0.1524" layer="21" curve="-180"/>
              <pad name="1" x="-3.81" y="8.89" drill="0.8" shape="long"/>
              <pad name="2" x="-3.81" y="6.35" drill="0.8" shape="long"/>
              <pad name="7" x="-3.81" y="-6.35" drill="0.8" shape="long"/>
              <pad name="8" x="-3.81" y="-8.89" drill="0.8" shape="long"/>
              <pad name="3" x="-3.81" y="3.81" drill="0.8" shape="long"/>
              <pad name="4" x="-3.81" y="1.27" drill="0.8" shape="long"/>
              <pad name="6" x="-3.81" y="-3.81" drill="0.8" shape="long"/>
              <pad name="5" x="-3.81" y="-1.27" drill="0.8" shape="long"/>
              <pad name="9" x="3.81" y="-8.89" drill="0.8" shape="long"/>
              <pad name="10" x="3.81" y="-6.35" drill="0.8" shape="long"/>
              <pad name="11" x="3.81" y="-3.81" drill="0.8" shape="long"/>
              <pad name="12" x="3.81" y="-1.27" drill="0.8" shape="long"/>
              <pad name="13" x="3.81" y="1.27" drill="0.8" shape="long"/>
              <pad name="14" x="3.81" y="3.81" drill="0.8" shape="long"/>
              <text x="-3.048" y="10.541" size="1.016" layer="25" ratio="10">&gt;NAME</text>
              <text x="0" y="0" size="0.8128" layer="27" ratio="10" rot="R180" align="center">&gt;VALUE</text>
              <pad name="15" x="3.81" y="6.35" drill="0.8" shape="long"/>
              <pad name="16" x="3.81" y="8.89" drill="0.8" shape="long"/>
            </package>
          </packages>
          <symbols>
            <symbol name="OPAMP">
              <wire x1="-5.08" y1="5.08" x2="-5.08" y2="-5.08" width="0.4064" layer="94"/>
              <wire x1="-5.08" y1="-5.08" x2="5.08" y2="0" width="0.4064" layer="94"/>
              <wire x1="5.08" y1="0" x2="-5.08" y2="5.08" width="0.4064" layer="94"/>
              <wire x1="-3.81" y1="3.175" x2="-3.81" y2="1.905" width="0.1524" layer="94"/>
              <wire x1="-4.445" y1="2.54" x2="-3.175" y2="2.54" width="0.1524" layer="94"/>
              <wire x1="-4.445" y1="-2.54" x2="-3.175" y2="-2.54" width="0.1524" layer="94"/>
              <text x="2.54" y="3.175" size="1.27" layer="95">&gt;NAME</text>
              <text x="2.54" y="-5.08" size="1.27" layer="96">&gt;VALUE</text>
              <pin name="-IN" x="-7.62" y="-2.54" visible="pad" length="short" direction="in"/>
              <pin name="+IN" x="-7.62" y="2.54" visible="pad" length="short" direction="in"/>
              <pin name="OUT" x="7.62" y="0" visible="pad" length="short" direction="out" rot="R180"/>
            </symbol>
            <symbol name="PWR+-">
              <text x="1.27" y="3.175" size="0.8128" layer="93" rot="R90">V+</text>
              <text x="1.27" y="-4.445" size="0.8128" layer="93" rot="R90">V-</text>
              <pin name="V+" x="0" y="5.08" visible="off" length="short" direction="pwr" rot="R270"/>
              <pin name="V-" x="0" y="-5.08" visible="off" length="short" direction="pwr" rot="R90"/>
              <text x="-1.27" y="3.81" size="1.778" layer="94" font="vector" align="center">+</text>
              <text x="-1.27" y="-3.81" size="1.778" layer="94" font="vector" align="center">-</text>
            </symbol>
            <symbol name="2NUL">
              <wire x1="-3.81" y1="3.175" x2="-3.81" y2="1.905" width="0.1524" layer="94"/>
              <wire x1="-4.445" y1="2.54" x2="-3.175" y2="2.54" width="0.1524" layer="94"/>
              <wire x1="-4.445" y1="-2.54" x2="-3.175" y2="-2.54" width="0.1524" layer="94"/>
              <wire x1="-2.54" y1="5.08" x2="-2.54" y2="3.8862" width="0.1524" layer="94"/>
              <wire x1="0" y1="5.0546" x2="0" y2="2.6162" width="0.1524" layer="94"/>
              <wire x1="0" y1="-2.6162" x2="0" y2="-5.08" width="0.1524" layer="94"/>
              <wire x1="-2.54" y1="-3.9116" x2="-2.54" y2="-5.08" width="0.1524" layer="94"/>
              <wire x1="5.08" y1="2.54" x2="2.6162" y2="1.3208" width="0.1524" layer="94"/>
              <wire x1="5.08" y1="0" x2="-5.08" y2="5.08" width="0.4064" layer="94"/>
              <wire x1="-5.08" y1="5.08" x2="-5.08" y2="-5.08" width="0.4064" layer="94"/>
              <wire x1="-5.08" y1="-5.08" x2="5.08" y2="0" width="0.4064" layer="94"/>
              <text x="2.54" y="5.715" size="1.016" layer="95">&gt;NAME</text>
              <text x="2.54" y="-5.08" size="0.8128" layer="96">&gt;VALUE</text>
              <text x="1.27" y="3.175" size="0.8128" layer="93" rot="R90">ON1</text>
              <text x="3.175" y="2.54" size="0.8128" layer="93" rot="R90">ON2</text>
              <text x="1.27" y="-5.08" size="0.8128" layer="93" rot="R90">OPT</text>
              <text x="-1.27" y="4.445" size="0.8128" layer="93" rot="R90">V+</text>
              <text x="-1.27" y="-5.715" size="0.8128" layer="93" rot="R90">V-</text>
              <pin name="ON2" x="7.62" y="2.54" visible="pad" length="short" direction="pas" rot="R180"/>
              <pin name="ON1" x="0" y="7.62" visible="pad" length="short" direction="pas" rot="R270"/>
              <pin name="-IN" x="-7.62" y="-2.54" visible="pad" length="short" direction="in"/>
              <pin name="+IN" x="-7.62" y="2.54" visible="pad" length="short" direction="in"/>
              <pin name="OUT" x="7.62" y="0" visible="pad" length="short" direction="out" rot="R180"/>
              <pin name="OPT" x="0" y="-7.62" visible="pad" length="short" direction="pas" rot="R90"/>
              <pin name="V+" x="-2.54" y="7.62" visible="pad" length="short" direction="pwr" rot="R270"/>
              <pin name="V-" x="-2.54" y="-7.62" visible="pad" length="short" direction="pwr" rot="R90"/>
            </symbol>
            <symbol name="OTA">
              <wire x1="-7.62" y1="7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94"/>
              <wire x1="-7.62" y1="-7.62" x2="7.62" y2="0" width="0.254" layer="94"/>
              <wire x1="7.62" y1="0" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
              <wire x1="-6.35" y1="5.715" x2="-6.35" y2="4.445" width="0.1524" layer="94"/>
              <wire x1="-6.985" y1="5.08" x2="-5.715" y2="5.08" width="0.1524" layer="94"/>
              <wire x1="-6.985" y1="-5.08" x2="-5.715" y2="-5.08" width="0.1524" layer="94"/>
              <text x="-7.62" y="8.255" size="1.27" layer="95">&gt;NAME</text>
              <text x="-7.62" y="-8.255" size="1.27" layer="96" align="top-left">&gt;VALUE</text>
              <pin name="-IN" x="-10.16" y="-5.08" visible="pad" length="short" direction="in"/>
              <pin name="+IN" x="-10.16" y="5.08" visible="pad" length="short" direction="in"/>
              <pin name="OUT" x="15.24" y="0" visible="pad" length="short" direction="out" rot="R180"/>
              <polygon width="0" layer="94">
                <vertex x="-7.62" y="0.508" curve="90"/>
                <vertex x="-8.128" y="0" curve="90"/>
                <vertex x="-7.62" y="-0.508" curve="90"/>
                <vertex x="-7.112" y="0" curve="90"/>
              </polygon>
              <polygon width="0" layer="94">
                <vertex x="-7.62" y="3.81"/>
                <vertex x="-8.89" y="1.524"/>
                <vertex x="-6.35" y="1.524"/>
              </polygon>
              <wire x1="-8.89" y1="3.81" x2="-6.35" y2="3.81" width="0.254" layer="94"/>
              <polygon width="0" layer="94">
                <vertex x="-7.62" y="-3.81"/>
                <vertex x="-6.35" y="-1.524"/>
                <vertex x="-8.89" y="-1.524"/>
              </polygon>
              <wire x1="-6.35" y1="-3.81" x2="-8.89" y2="-3.81" width="0.254" layer="94"/>
              <pin name="DIODE/BIAS" x="-10.16" y="0" visible="pad" length="short" direction="in"/>
              <circle x="9.41605" y="0" radius="1.79605" width="0.254" layer="94"/>
              <circle x="10.90395" y="0" radius="1.79605" width="0.254" layer="94"/>
              <pin name="AMP/BIAS" x="10.16" y="-5.08" visible="pad" length="short" direction="in" rot="R90"/>
              <wire x1="10.16" y1="-2.54" x2="10.16" y2="-1.651" width="0.1524" layer="94"/>
            </symbol>
            <symbol name="OTABUF">
              <wire x1="0" y1="2.54" x2="-1.27" y2="2.54" width="0.1524" layer="94"/>
              <wire x1="-1.27" y1="2.54" x2="-3.7465" y2="1.30175" width="0.1524" layer="94"/>
              <wire x1="-2.3495" y1="-1.36525" x2="-1.8415" y2="-2.25425" width="0.1524" layer="94"/>
              <wire x1="-1.8415" y1="-2.25425" x2="-2.8575" y2="-2.38125" width="0.1524" layer="94"/>
              <wire x1="-2.8575" y1="-2.38125" x2="-2.3495" y2="-1.36525" width="0.1524" layer="94"/>
              <wire x1="-1.27" y1="-2.54" x2="-1.8415" y2="-2.25425" width="0.1524" layer="94"/>
              <wire x1="-1.8415" y1="-2.25425" x2="-3.502" y2="-1.424" width="0.1524" layer="94"/>
              <wire x1="-2.6035" y1="-2.25425" x2="-2.032" y2="-2.19075" width="0.3048" layer="94"/>
              <wire x1="-2.032" y1="-2.19075" x2="-2.3495" y2="-1.61925" width="0.3048" layer="94"/>
              <wire x1="-2.3495" y1="-1.61925" x2="-2.6035" y2="-2.12725" width="0.254" layer="94"/>
              <wire x1="-2.6035" y1="-2.12725" x2="-2.2225" y2="-2.12725" width="0.254" layer="94"/>
              <wire x1="-2.2225" y1="-2.12725" x2="-2.3495" y2="-1.87325" width="0.254" layer="94"/>
              <text x="-7.62" y="8.255" size="1.27" layer="95" ratio="10">&gt;NAME</text>
              <text x="-7.62" y="-8.255" size="1.27" layer="96" ratio="10" align="top-left">&gt;VALUE</text>
              <rectangle x1="-4.318" y1="-2.54" x2="-3.556" y2="2.54" layer="94"/>
              <pin name="BUFF/IN" x="-10.16" y="0" visible="pad" length="short" direction="in"/>
              <pin name="BUFF/OUT" x="5.08" y="-10.16" visible="pad" length="short" direction="out" rot="R90"/>
              <wire x1="5.08" y1="0" x2="3.81" y2="0" width="0.1524" layer="94"/>
              <wire x1="3.81" y1="0" x2="1.3335" y2="-1.23825" width="0.1524" layer="94"/>
              <wire x1="2.7305" y1="-3.90525" x2="3.2385" y2="-4.79425" width="0.1524" layer="94"/>
              <wire x1="3.2385" y1="-4.79425" x2="2.2225" y2="-4.92125" width="0.1524" layer="94"/>
              <wire x1="2.2225" y1="-4.92125" x2="2.7305" y2="-3.90525" width="0.1524" layer="94"/>
              <wire x1="3.81" y1="-5.08" x2="3.2385" y2="-4.79425" width="0.1524" layer="94"/>
              <wire x1="3.2385" y1="-4.79425" x2="1.578" y2="-3.964" width="0.1524" layer="94"/>
              <wire x1="2.4765" y1="-4.79425" x2="3.048" y2="-4.73075" width="0.3048" layer="94"/>
              <wire x1="3.048" y1="-4.73075" x2="2.7305" y2="-4.15925" width="0.3048" layer="94"/>
              <wire x1="2.7305" y1="-4.15925" x2="2.4765" y2="-4.66725" width="0.254" layer="94"/>
              <wire x1="2.4765" y1="-4.66725" x2="2.8575" y2="-4.66725" width="0.254" layer="94"/>
              <wire x1="2.8575" y1="-4.66725" x2="2.7305" y2="-4.41325" width="0.254" layer="94"/>
              <rectangle x1="0.762" y1="-5.08" x2="1.524" y2="0" layer="94"/>
              <wire x1="-1.27" y1="-2.54" x2="1.27" y2="-2.54" width="0.1524" layer="94"/>
              <polygon width="0.127" layer="94">
                <vertex x="0" y="2.286" curve="-90"/>
                <vertex x="-0.254" y="2.54" curve="-90"/>
                <vertex x="0" y="2.794" curve="-90"/>
                <vertex x="0.254" y="2.54" curve="-90"/>
              </polygon>
              <wire x1="0" y1="2.54" x2="5.08" y2="2.54" width="0.1524" layer="94"/>
              <wire x1="5.08" y1="2.54" x2="5.08" y2="0" width="0.1524" layer="94"/>
              <circle x="0" y="5.08" radius="0.762" width="0.1524" layer="94"/>
              <wire x1="0.508" y1="5.08" x2="-0.508" y2="5.08" width="0.127" layer="94"/>
              <wire x1="0" y1="5.588" x2="0" y2="4.572" width="0.127" layer="94"/>
              <wire x1="0" y1="4.318" x2="0" y2="2.54" width="0.1524" layer="94"/>
              <wire x1="-7.62" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94" style="shortdash"/>
              <wire x1="7.62" y1="-7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94" style="shortdash"/>
              <wire x1="-10.16" y1="0" x2="-3.81" y2="0" width="0.1524" layer="94"/>
              <wire x1="3.81" y1="-5.08" x2="5.08" y2="-5.08" width="0.1524" layer="94"/>
              <wire x1="5.08" y1="-5.08" x2="5.08" y2="-10.16" width="0.1524" layer="94"/>
              <wire x1="7.62" y1="7.62" x2="7.62" y2="-7.62" width="0.254" layer="94" style="shortdash"/>
              <wire x1="-7.62" y1="7.62" x2="-7.62" y2="-7.62" width="0.254" layer="94" style="shortdash"/>
            </symbol>
            <symbol name="PWR+-OTA">
              <text x="-1.27" y="4.445" size="0.8128" layer="93" rot="MR90">V+</text>
              <text x="-1.27" y="-5.715" size="0.8128" layer="93" rot="MR90">V-</text>
              <pin name="V+" x="0" y="6.35" visible="off" length="short" direction="pwr" rot="R270"/>
              <pin name="V-" x="0" y="-6.35" visible="off" length="short" direction="pwr" rot="R90"/>
              <text x="1.27" y="5.08" size="1.778" layer="94" font="vector" rot="MR0" align="center">+</text>
              <text x="1.27" y="-5.08" size="1.778" layer="94" font="vector" rot="MR0" align="center">-</text>
            </symbol>
          </symbols>
          <devicesets>
            <deviceset name="TL071" prefix="IC">
              <description>Low-Noise JFET-Input Operational Amplifiers&lt;br&gt;
&lt;i&gt;http://www.ti.com/lit/ds/symlink/tl072.pdf&lt;/i&gt;</description>
              <gates>
                <gate name="A" symbol="2NUL" x="5.08" y="0"/>
              </gates>
              <devices>
                <device name="P" package="DIL08">
                  <connects>
                    <connect gate="A" pin="+IN" pad="3"/>
                    <connect gate="A" pin="-IN" pad="2"/>
                    <connect gate="A" pin="ON1" pad="1"/>
                    <connect gate="A" pin="ON2" pad="5"/>
                    <connect gate="A" pin="OPT" pad="8"/>
                    <connect gate="A" pin="OUT" pad="6"/>
                    <connect gate="A" pin="V+" pad="7"/>
                    <connect gate="A" pin="V-" pad="4"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="D" package="SO08">
                  <connects>
                    <connect gate="A" pin="+IN" pad="3"/>
                    <connect gate="A" pin="-IN" pad="2"/>
                    <connect gate="A" pin="ON1" pad="1"/>
                    <connect gate="A" pin="ON2" pad="5"/>
                    <connect gate="A" pin="OPT" pad="8"/>
                    <connect gate="A" pin="OUT" pad="6"/>
                    <connect gate="A" pin="V+" pad="7"/>
                    <connect gate="A" pin="V-" pad="4"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="LM13600">
              <gates>
                <gate name="A" symbol="OTA" x="0" y="22.86"/>
                <gate name="B" symbol="OTA" x="0" y="2.54"/>
                <gate name="A*" symbol="OTABUF" x="35.56" y="22.86"/>
                <gate name="B*" symbol="OTABUF" x="35.56" y="2.54"/>
                <gate name="G$5" symbol="PWR+-OTA" x="2.54" y="22.86"/>
              </gates>
              <devices>
                <device name="" package="DIL16">
                  <connects>
                    <connect gate="A" pin="+IN" pad="3"/>
                    <connect gate="A" pin="-IN" pad="4"/>
                    <connect gate="A" pin="AMP/BIAS" pad="1"/>
                    <connect gate="A" pin="DIODE/BIAS" pad="2"/>
                    <connect gate="A" pin="OUT" pad="5"/>
                    <connect gate="A*" pin="BUFF/IN" pad="7"/>
                    <connect gate="A*" pin="BUFF/OUT" pad="8"/>
                    <connect gate="B" pin="+IN" pad="14"/>
                    <connect gate="B" pin="-IN" pad="13"/>
                    <connect gate="B" pin="AMP/BIAS" pad="16"/>
                    <connect gate="B" pin="DIODE/BIAS" pad="15"/>
                    <connect gate="B" pin="OUT" pad="12"/>
                    <connect gate="B*" pin="BUFF/IN" pad="9"/>
                    <connect gate="B*" pin="BUFF/OUT" pad="10"/>
                    <connect gate="G$5" pin="V+" pad="11"/>
                    <connect gate="G$5" pin="V-" pad="6"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="TL072" prefix="IC">
              <description>Low-Noise JFET-Input Operational Amplifiers&lt;br&gt;
&lt;i&gt;http://www.ti.com/lit/ds/symlink/tl072.pdf&lt;/i&gt;</description>
              <gates>
                <gate name="A" symbol="OPAMP" x="15.24" y="10.16" swaplevel="1"/>
                <gate name="B" symbol="OPAMP" x="15.24" y="-12.7" swaplevel="1"/>
                <gate name="P" symbol="PWR+-" x="15.24" y="10.16" addlevel="request"/>
              </gates>
              <devices>
                <device name="P" package="DIL08">
                  <connects>
                    <connect gate="A" pin="+IN" pad="3"/>
                    <connect gate="A" pin="-IN" pad="2"/>
                    <connect gate="A" pin="OUT" pad="1"/>
                    <connect gate="B" pin="+IN" pad="5"/>
                    <connect gate="B" pin="-IN" pad="6"/>
                    <connect gate="B" pin="OUT" pad="7"/>
                    <connect gate="P" pin="V+" pad="8"/>
                    <connect gate="P" pin="V-" pad="4"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="D" package="SO08">
                  <connects>
                    <connect gate="A" pin="+IN" pad="3"/>
                    <connect gate="A" pin="-IN" pad="2"/>
                    <connect gate="A" pin="OUT" pad="1"/>
                    <connect gate="B" pin="+IN" pad="5"/>
                    <connect gate="B" pin="-IN" pad="6"/>
                    <connect gate="B" pin="OUT" pad="7"/>
                    <connect gate="P" pin="V+" pad="8"/>
                    <connect gate="P" pin="V-" pad="4"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
          </devicesets>
        </library>
        <library name="t">
          <description>&lt;b&gt;Transistors&lt;/b&gt;&lt;p&gt;
</description>
          <packages>
            <package name="TO92">
              <description>&lt;b&gt;TO 92&lt;/b&gt;</description>
              <pad name="3" x="0" y="-2.54" drill="0.8" shape="octagon" rot="R270"/>
              <pad name="1" x="0" y="2.54" drill="0.8" shape="octagon" rot="R270"/>
              <pad name="2" x="0" y="0" drill="0.8" shape="octagon" rot="R270"/>
              <text x="-1.27" y="2.54" size="0.8128" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="1.27" y="0" size="0.8128" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
              <wire x1="-1.651" y1="2.095" x2="-1.651" y2="-2.095" width="0.127" layer="21"/>
              <wire x1="1.1359" y1="2.413" x2="1.1359" y2="-2.413" width="0.127" layer="21" curve="-129.583"/>
              <wire x1="-0.127" y1="-2.664" x2="-0.127" y2="2.664" width="0.127" layer="21"/>
              <wire x1="1.1359" y1="2.413" x2="-0.127" y2="2.664" width="0.127" layer="21" curve="27.9376"/>
              <wire x1="-0.127" y1="2.664" x2="-1.1359" y2="2.413" width="0.127" layer="21" curve="22.4788"/>
              <wire x1="-1.1359" y1="2.4135" x2="-1.651" y2="2.095" width="0.127" layer="21" curve="13.0385"/>
              <wire x1="-1.1359" y1="-2.413" x2="-0.127" y2="-2.664" width="0.127" layer="21" curve="22.4788"/>
              <wire x1="-0.127" y1="-2.664" x2="1.1359" y2="-2.413" width="0.127" layer="21" curve="27.9376"/>
              <wire x1="-1.651" y1="-2.095" x2="-1.1118" y2="-2.4247" width="0.127" layer="21" curve="13.6094"/>
              <text x="-0.635" y="1.905" size="0.3048" layer="21" rot="R180">1</text>
            </package>
            <package name="TO92-EBC">
              <description>&lt;b&gt;TO-92&lt;/b&gt; Pads In Line E B C from top&lt;p&gt;</description>
              <wire x1="1.651" y1="-2.095" x2="1.651" y2="2.095" width="0.127" layer="21"/>
              <wire x1="-1.1359" y1="-2.413" x2="-1.1359" y2="2.413" width="0.127" layer="21" curve="-129.583"/>
              <wire x1="0.127" y1="1.136" x2="0.127" y2="-1.136" width="0.127" layer="51"/>
              <wire x1="-1.1359" y1="-2.413" x2="0.127" y2="-2.664" width="0.127" layer="51" curve="27.9376"/>
              <wire x1="0.127" y1="-2.664" x2="1.1359" y2="-2.413" width="0.127" layer="51" curve="22.4788"/>
              <wire x1="0.127" y1="-1.404" x2="0.127" y2="-2.664" width="0.127" layer="51"/>
              <wire x1="1.1359" y1="-2.4135" x2="1.651" y2="-2.095" width="0.127" layer="21" curve="13.0385"/>
              <wire x1="0.127" y1="-1.136" x2="0.127" y2="-1.404" width="0.127" layer="21"/>
              <wire x1="1.1359" y1="2.413" x2="0.127" y2="2.664" width="0.127" layer="51" curve="22.4788"/>
              <wire x1="0.127" y1="2.664" x2="-1.1359" y2="2.413" width="0.127" layer="51" curve="27.9376"/>
              <wire x1="0.127" y1="2.664" x2="0.127" y2="1.404" width="0.127" layer="51"/>
              <wire x1="0.127" y1="1.404" x2="0.127" y2="1.136" width="0.127" layer="21"/>
              <wire x1="1.651" y1="2.095" x2="1.1118" y2="2.4247" width="0.127" layer="21" curve="13.6094"/>
              <pad name="C" x="0" y="2.54" drill="0.8" shape="octagon" rot="R180"/>
              <pad name="E" x="0" y="-2.54" drill="0.8" shape="octagon" rot="R180"/>
              <pad name="B" x="0" y="0" drill="0.8" shape="octagon" rot="R180"/>
              <text x="1.27" y="-2.032" size="0.3048" layer="21" rot="R90">E</text>
              <text x="1.27" y="0.508" size="0.3048" layer="21" rot="R90">B</text>
              <text x="1.27" y="3.048" size="0.3048" layer="21" rot="R90">C</text>
              <text x="-1.27" y="2.54" size="0.8128" layer="25" ratio="10" align="bottom-right">&gt;NAME</text>
              <text x="-1.27" y="0" size="0.8128" layer="27" ratio="10" rot="R270" align="center">&gt;VALUE</text>
            </package>
          </packages>
          <symbols>
            <symbol name="PNP">
              <wire x1="2.0861" y1="1.6779" x2="1.5781" y2="2.5941" width="0.1524" layer="94"/>
              <wire x1="1.5781" y1="2.5941" x2="0.5159" y2="1.478" width="0.1524" layer="94"/>
              <wire x1="0.5159" y1="1.478" x2="2.0861" y2="1.6779" width="0.1524" layer="94"/>
              <wire x1="2.54" y1="2.54" x2="1.808" y2="2.1239" width="0.1524" layer="94"/>
              <wire x1="2.54" y1="-2.54" x2="0.508" y2="-1.524" width="0.1524" layer="94"/>
              <wire x1="1.905" y1="1.778" x2="1.524" y2="2.413" width="0.254" layer="94"/>
              <wire x1="1.524" y1="2.413" x2="0.762" y2="1.651" width="0.254" layer="94"/>
              <wire x1="0.762" y1="1.651" x2="1.778" y2="1.778" width="0.254" layer="94"/>
              <wire x1="1.778" y1="1.778" x2="1.524" y2="2.159" width="0.254" layer="94"/>
              <wire x1="1.524" y1="2.159" x2="1.143" y2="1.905" width="0.254" layer="94"/>
              <wire x1="1.143" y1="1.905" x2="1.524" y2="1.905" width="0.254" layer="94"/>
              <text x="3.81" y="2.54" size="1.27" layer="95" ratio="10">&gt;NAME</text>
              <text x="3.81" y="-2.54" size="1.27" layer="96" ratio="10" align="top-left">&gt;VALUE</text>
              <rectangle x1="-0.254" y1="-2.54" x2="0.508" y2="2.54" layer="94"/>
              <pin name="B" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
              <pin name="E" x="2.54" y="5.08" visible="off" length="short" direction="pas" rot="R270"/>
              <pin name="C" x="2.54" y="-5.08" visible="off" length="short" direction="pas" rot="R90"/>
              <circle x="1.27" y="0" radius="3.5921" width="0.254" layer="94"/>
            </symbol>
          </symbols>
          <devicesets>
            <deviceset name="2N3906" prefix="T">
              <gates>
                <gate name="G$1" symbol="PNP" x="-2.54" y="0"/>
              </gates>
              <devices>
                <device name="" package="TO92">
                  <connects>
                    <connect gate="G$1" pin="B" pad="2"/>
                    <connect gate="G$1" pin="C" pad="3"/>
                    <connect gate="G$1" pin="E" pad="1"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="TO92-EBC" package="TO92-EBC">
                  <connects>
                    <connect gate="G$1" pin="B" pad="B"/>
                    <connect gate="G$1" pin="C" pad="C"/>
                    <connect gate="G$1" pin="E" pad="E"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
          </devicesets>
        </library>
        <library name="c">
          <description>&lt;b&gt;Potentiometers&lt;/b&gt;&lt;p&gt;
 Beckman, Copal, Piher, Spectrol, Schukat&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
          <packages>
            <package name="C025-015X024">
              <wire x1="0" y1="1.397" x2="0.635" y2="0.889" width="0.127" layer="21" curve="-90"/>
              <wire x1="0.635" y1="-0.889" x2="0.635" y2="0.889" width="0.127" layer="21"/>
              <wire x1="0.635" y1="-0.889" x2="0" y2="-1.397" width="0.127" layer="21" curve="-90"/>
              <wire x1="0" y1="-1.016" x2="0" y2="-0.3937" width="0.0762" layer="51"/>
              <wire x1="0" y1="1.016" x2="0" y2="0.381" width="0.0762" layer="51"/>
              <pad name="2" x="0" y="-1.27" drill="0.8" rot="R270" stop="no"/>
              <text x="-1.016" y="0" size="0.6096" layer="25" align="center-right">&gt;NAME</text>
              <text x="0.508" y="-2.413" size="0.6096" layer="27" rot="R180">&gt;VALUE</text>
              <pad name="1" x="0" y="1.27" drill="0.8" rot="R90" stop="no"/>
              <wire x1="0" y1="-1.397" x2="-0.635" y2="-0.889" width="0.127" layer="21" curve="-90"/>
              <wire x1="-0.635" y1="0.889" x2="-0.635" y2="-0.889" width="0.127" layer="21"/>
              <wire x1="-0.635" y1="0.889" x2="0" y2="1.397" width="0.127" layer="21" curve="-90"/>
              <rectangle x1="-0.381" y1="0.0635" x2="0.381" y2="0.254" layer="21"/>
              <rectangle x1="-0.381" y1="-0.254" x2="0.381" y2="-0.0635" layer="21" rot="R180"/>
            </package>
            <package name="C050-020X044">
              <wire x1="-1.778" y1="-0.635" x2="1.778" y2="-0.635" width="0.1016" layer="21"/>
              <wire x1="-2.159" y1="-0.254" x2="-2.159" y2="0.254" width="0.1016" layer="21"/>
              <pad name="2" x="2.54" y="0" drill="0.8" rot="R180"/>
              <text x="-2.159" y="1.27" size="0.6096" layer="25" align="center-left">&gt;NAME</text>
              <text x="0" y="-1.27" size="0.6096" layer="27" rot="R180" align="center-left">&gt;VALUE</text>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <wire x1="-2.159" y1="0.254" x2="-1.778" y2="0.635" width="0.1016" layer="21" curve="-90"/>
              <wire x1="1.778" y1="0.635" x2="-1.778" y2="0.635" width="0.1016" layer="21"/>
              <wire x1="-2.54" y1="0" x2="-0.3937" y2="0" width="0.0762" layer="51"/>
              <rectangle x1="-0.254" y1="-0.127" x2="0.762" y2="0.127" layer="21" rot="R270"/>
              <rectangle x1="-0.762" y1="-0.127" x2="0.254" y2="0.127" layer="21" rot="R90"/>
              <wire x1="2.54" y1="0" x2="0.381" y2="0" width="0.0762" layer="51"/>
              <wire x1="-2.159" y1="-0.254" x2="-1.778" y2="-0.635" width="0.1016" layer="21" curve="90"/>
              <wire x1="2.159" y1="0.254" x2="2.159" y2="-0.254" width="0.1016" layer="21"/>
              <wire x1="2.159" y1="-0.254" x2="1.778" y2="-0.635" width="0.1016" layer="21" curve="-90"/>
              <wire x1="2.159" y1="0.254" x2="1.778" y2="0.635" width="0.1016" layer="21" curve="90"/>
            </package>
            <package name="C075-020X044">
              <wire x1="-1.778" y1="-0.635" x2="1.778" y2="-0.635" width="0.1016" layer="21"/>
              <wire x1="-2.159" y1="-0.254" x2="-2.159" y2="0.254" width="0.1016" layer="21"/>
              <pad name="2" x="3.81" y="0" drill="0.8" rot="R180"/>
              <text x="-2.159" y="1.27" size="0.6096" layer="25" align="center-left">&gt;NAME</text>
              <text x="0" y="-1.27" size="0.6096" layer="27" align="center-left">&gt;VALUE</text>
              <pad name="1" x="-3.81" y="0" drill="0.8"/>
              <wire x1="-2.159" y1="0.254" x2="-1.778" y2="0.635" width="0.1016" layer="21" curve="-90"/>
              <wire x1="1.778" y1="0.635" x2="-1.778" y2="0.635" width="0.1016" layer="21"/>
              <wire x1="-3.81" y1="0" x2="-0.3937" y2="0" width="0.0762" layer="51"/>
              <rectangle x1="-0.254" y1="-0.127" x2="0.762" y2="0.127" layer="21" rot="R270"/>
              <rectangle x1="-0.762" y1="-0.127" x2="0.254" y2="0.127" layer="21" rot="R90"/>
              <wire x1="3.81" y1="0" x2="0.381" y2="0" width="0.0762" layer="51"/>
              <wire x1="-2.159" y1="-0.254" x2="-1.778" y2="-0.635" width="0.1016" layer="21" curve="90"/>
              <wire x1="2.159" y1="0.254" x2="2.159" y2="-0.254" width="0.1016" layer="21"/>
              <wire x1="2.159" y1="-0.254" x2="1.778" y2="-0.635" width="0.1016" layer="21" curve="-90"/>
              <wire x1="2.159" y1="0.254" x2="1.778" y2="0.635" width="0.1016" layer="21" curve="90"/>
            </package>
            <package name="C075-020X044/V">
              <wire x1="-0.635" y1="1.778" x2="-0.635" y2="-1.778" width="0.1016" layer="21"/>
              <wire x1="-0.254" y1="2.159" x2="0.254" y2="2.159" width="0.1016" layer="21"/>
              <pad name="2" x="0" y="-3.81" drill="0.8" rot="R90"/>
              <text x="-1.016" y="2.54" size="0.6096" layer="25" rot="R180" align="center-left">&gt;NAME</text>
              <text x="0.635" y="-2.54" size="0.6096" layer="27" align="center-left">&gt;VALUE</text>
              <pad name="1" x="0" y="3.81" drill="0.8" rot="R270"/>
              <wire x1="0.254" y1="2.159" x2="0.635" y2="1.778" width="0.1016" layer="21" curve="-90"/>
              <wire x1="0.635" y1="-1.778" x2="0.635" y2="1.778" width="0.1016" layer="21"/>
              <wire x1="0" y1="3.81" x2="0" y2="0.3937" width="0.0762" layer="51"/>
              <rectangle x1="-0.508" y1="-0.381" x2="0.508" y2="-0.127" layer="21" rot="R180"/>
              <rectangle x1="-0.508" y1="0.127" x2="0.508" y2="0.381" layer="21"/>
              <wire x1="0" y1="-3.81" x2="0" y2="-0.381" width="0.0762" layer="51"/>
              <wire x1="-0.254" y1="2.159" x2="-0.635" y2="1.778" width="0.1016" layer="21" curve="90"/>
              <wire x1="0.254" y1="-2.159" x2="-0.254" y2="-2.159" width="0.1016" layer="21"/>
              <wire x1="-0.254" y1="-2.159" x2="-0.635" y2="-1.778" width="0.1016" layer="21" curve="-90"/>
              <wire x1="0.254" y1="-2.159" x2="0.635" y2="-1.778" width="0.1016" layer="21" curve="90"/>
            </package>
            <package name="C050-020X044/V">
              <wire x1="-0.635" y1="1.778" x2="-0.635" y2="-1.778" width="0.1016" layer="21"/>
              <wire x1="-0.254" y1="2.159" x2="0.254" y2="2.159" width="0.1016" layer="21"/>
              <pad name="2" x="0" y="-2.54" drill="0.8" rot="R90"/>
              <text x="-1.016" y="1.27" size="0.6096" layer="25" align="bottom-right">&gt;NAME</text>
              <text x="1.016" y="-1.27" size="0.6096" layer="27" align="top-left">&gt;VALUE</text>
              <pad name="1" x="0" y="2.54" drill="0.8" rot="R270"/>
              <wire x1="0.254" y1="2.159" x2="0.635" y2="1.778" width="0.1016" layer="21" curve="-90"/>
              <wire x1="0.635" y1="-1.778" x2="0.635" y2="1.778" width="0.1016" layer="21"/>
              <wire x1="0" y1="2.54" x2="0" y2="0.3937" width="0.0762" layer="51"/>
              <rectangle x1="-0.508" y1="-0.381" x2="0.508" y2="-0.127" layer="21" rot="R180"/>
              <rectangle x1="-0.508" y1="0.127" x2="0.508" y2="0.381" layer="21"/>
              <wire x1="0" y1="-2.54" x2="0" y2="-0.381" width="0.0762" layer="51"/>
              <wire x1="-0.254" y1="2.159" x2="-0.635" y2="1.778" width="0.1016" layer="21" curve="90"/>
              <wire x1="0.254" y1="-2.159" x2="-0.254" y2="-2.159" width="0.1016" layer="21"/>
              <wire x1="-0.254" y1="-2.159" x2="-0.635" y2="-1.778" width="0.1016" layer="21" curve="-90"/>
              <wire x1="0.254" y1="-2.159" x2="0.635" y2="-1.778" width="0.1016" layer="21" curve="90"/>
            </package>
            <package name="C050-024X044">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 2.4 x 4.4 mm</description>
              <wire x1="-2.159" y1="-0.635" x2="-2.159" y2="0.635" width="0.1524" layer="51"/>
              <wire x1="-2.159" y1="0.635" x2="-1.651" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.159" y1="-0.635" x2="-1.651" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.651" y1="1.143" x2="-1.651" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-0.635" x2="2.159" y2="0.635" width="0.1524" layer="51"/>
              <wire x1="1.651" y1="-1.143" x2="-1.651" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="1.651" y1="1.143" x2="2.159" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <wire x1="1.651" y1="-1.143" x2="2.159" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.3048" y1="0.762" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.762" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0.762" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.762" width="0.3048" layer="21"/>
              <wire x1="1.27" y1="0" x2="0.3302" y2="0" width="0.1524" layer="21"/>
              <wire x1="-1.27" y1="0" x2="-0.3048" y2="0" width="0.1524" layer="21"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="0" y="1.524" size="0.6096" layer="25" align="bottom-center">&gt;NAME</text>
              <text x="0" y="-1.524" size="0.6096" layer="27" align="top-center">&gt;VALUE</text>
              <rectangle x1="2.159" y1="-0.381" x2="2.54" y2="0.381" layer="51"/>
              <rectangle x1="-2.54" y1="-0.381" x2="-2.159" y2="0.381" layer="51"/>
            </package>
            <package name="C050-024X044/V">
              <wire x1="-0.635" y1="2.159" x2="0.635" y2="2.159" width="0.1524" layer="51"/>
              <wire x1="0.635" y1="2.159" x2="1.143" y2="1.651" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.635" y1="2.159" x2="-1.143" y2="1.651" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.143" y1="-1.651" x2="1.143" y2="1.651" width="0.1524" layer="21"/>
              <wire x1="-0.635" y1="-2.159" x2="0.635" y2="-2.159" width="0.1524" layer="51"/>
              <wire x1="-1.143" y1="-1.651" x2="-1.143" y2="1.651" width="0.1524" layer="21"/>
              <wire x1="1.143" y1="-1.651" x2="0.635" y2="-2.159" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.143" y1="-1.651" x2="-0.635" y2="-2.159" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0.3048" x2="0" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0" y1="0.3048" x2="-0.762" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0.762" y1="-0.3302" x2="0" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-0.3302" x2="-0.762" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-1.27" x2="0" y2="-0.3302" width="0.1524" layer="21"/>
              <wire x1="0" y1="1.27" x2="0" y2="0.3048" width="0.1524" layer="21"/>
              <pad name="1" x="0" y="2.54" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-2.54" drill="0.8" rot="R270"/>
              <text x="-1.016" y="2.286" size="0.6096" layer="25" align="bottom-right">&gt;NAME</text>
              <text x="-1.016" y="-2.286" size="0.6096" layer="27" align="top-right">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.7305" x2="0.1905" y2="-1.9685" layer="51" rot="R270"/>
              <rectangle x1="-0.1905" y1="1.9685" x2="0.1905" y2="2.7305" layer="51" rot="R270"/>
            </package>
            <package name="C050-020X036">
              <wire x1="-1.524" y1="0" x2="-0.889" y2="0.635" width="0.127" layer="21" curve="-90"/>
              <wire x1="0.889" y1="0.635" x2="-0.889" y2="0.635" width="0.127" layer="21"/>
              <wire x1="0.889" y1="0.635" x2="1.524" y2="0" width="0.127" layer="21" curve="-90"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-0.254" y="0.8255" size="0.6096" layer="25" align="bottom-right">&gt;NAME</text>
              <text x="0.254" y="0.8255" size="0.6096" layer="27">&gt;VALUE</text>
              <pad name="1" x="-2.54" y="0" drill="0.8" rot="R180"/>
              <wire x1="1.524" y1="0" x2="0.889" y2="-0.635" width="0.127" layer="21" curve="-90"/>
              <wire x1="-0.889" y1="-0.635" x2="0.889" y2="-0.635" width="0.127" layer="21"/>
              <wire x1="-0.889" y1="-0.635" x2="-1.524" y2="0" width="0.127" layer="21" curve="-90"/>
              <wire x1="2.54" y1="0" x2="0.3937" y2="0" width="0.0762" layer="51"/>
              <rectangle x1="-0.762" y1="-0.127" x2="0.254" y2="0.127" layer="21" rot="R90"/>
              <rectangle x1="-0.254" y1="-0.127" x2="0.762" y2="0.127" layer="21" rot="R270"/>
              <wire x1="-2.54" y1="0" x2="-0.381" y2="0" width="0.0762" layer="51"/>
              <wire x1="-1.524" y1="0" x2="-0.889" y2="0.635" width="0.127" layer="21" curve="-90"/>
            </package>
            <package name="C050-020X036/V">
              <wire x1="0" y1="-1.524" x2="-0.635" y2="-0.889" width="0.127" layer="21" curve="-90"/>
              <wire x1="-0.635" y1="0.889" x2="-0.635" y2="-0.889" width="0.127" layer="21"/>
              <wire x1="-0.635" y1="0.889" x2="0" y2="1.524" width="0.127" layer="21" curve="-90"/>
              <pad name="2" x="0" y="2.54" drill="0.8" rot="R90"/>
              <text x="-0.8128" y="0" size="0.6096" layer="25" rot="R90" align="bottom-center">&gt;NAME</text>
              <text x="0.8128" y="0" size="0.6096" layer="27" rot="R90" align="top-center">&gt;VALUE</text>
              <pad name="1" x="0" y="-2.54" drill="0.8" rot="R270"/>
              <wire x1="0" y1="1.524" x2="0.635" y2="0.889" width="0.127" layer="21" curve="-90"/>
              <wire x1="0.635" y1="-0.889" x2="0.635" y2="0.889" width="0.127" layer="21"/>
              <wire x1="0.635" y1="-0.889" x2="0" y2="-1.524" width="0.127" layer="21" curve="-90"/>
              <wire x1="0" y1="2.54" x2="0" y2="0.3937" width="0.0762" layer="51"/>
              <rectangle x1="-0.508" y1="-0.381" x2="0.508" y2="-0.127" layer="21" rot="R180"/>
              <rectangle x1="-0.508" y1="0.127" x2="0.508" y2="0.381" layer="21"/>
              <wire x1="0" y1="-2.54" x2="0" y2="-0.381" width="0.0762" layer="51"/>
              <wire x1="0" y1="-1.524" x2="-0.635" y2="-0.889" width="0.127" layer="21" curve="-90"/>
            </package>
            <package name="C025-024X044">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 2.4 x 4.4 mm</description>
              <wire x1="-2.159" y1="-0.635" x2="-2.159" y2="0.635" width="0.1524" layer="21"/>
              <wire x1="-2.159" y1="0.635" x2="-1.651" y2="1.143" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.159" y1="-0.635" x2="-1.651" y2="-1.143" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.651" y1="1.143" x2="-1.651" y2="1.143" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-0.635" x2="2.159" y2="0.635" width="0.1524" layer="21"/>
              <wire x1="1.651" y1="-1.143" x2="-1.651" y2="-1.143" width="0.1524" layer="21"/>
              <wire x1="1.651" y1="1.143" x2="2.159" y2="0.635" width="0.1524" layer="21" curve="-90"/>
              <wire x1="1.651" y1="-1.143" x2="2.159" y2="-0.635" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.3048" y1="0.762" x2="-0.3048" y2="-0.762" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0.762" x2="0.3302" y2="-0.762" width="0.3048" layer="21"/>
              <wire x1="1.27" y1="0" x2="0.3302" y2="0" width="0.1524" layer="51"/>
              <wire x1="-1.27" y1="0" x2="-0.3048" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-1.778" y="1.397" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.778" y="-2.667" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-025X050">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 2.5 x 5 mm</description>
              <wire x1="-2.159" y1="1.27" x2="2.159" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-1.27" x2="-2.159" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.016" x2="2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="1.27" x2="2.413" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.413" y1="1.016" x2="-2.159" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="2.159" y1="-1.27" x2="2.413" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="-2.413" y1="-1.016" x2="-2.159" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0" x2="0.381" y2="0" width="0.1524" layer="51"/>
              <wire x1="0.381" y1="0" x2="0.254" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="0.762" width="0.254" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0.762" x2="-0.254" y2="0" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.381" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="0" x2="-0.762" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.286" y="1.524" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-2.286" y="-2.794" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-030X050">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 3 x 5 mm</description>
              <wire x1="-2.159" y1="1.524" x2="2.159" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-1.524" x2="-2.159" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.27" x2="2.413" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.27" x2="-2.413" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="1.524" x2="2.413" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.413" y1="1.27" x2="-2.159" y2="1.524" width="0.1524" layer="21" curve="-90"/>
              <wire x1="2.159" y1="-1.524" x2="2.413" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-2.413" y1="-1.27" x2="-2.159" y2="-1.524" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0" x2="0.381" y2="0" width="0.1524" layer="51"/>
              <wire x1="0.381" y1="0" x2="0.254" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="0.762" width="0.254" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0.762" x2="-0.254" y2="0" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.381" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="0" x2="-0.762" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.286" y="1.905" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-2.286" y="-3.048" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-040X050">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 4 x 5 mm</description>
              <wire x1="-2.159" y1="1.905" x2="2.159" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-1.905" x2="-2.159" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="1.651" x2="2.413" y2="-1.651" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="1.651" x2="-2.413" y2="-1.651" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="1.905" x2="2.413" y2="1.651" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.413" y1="1.651" x2="-2.159" y2="1.905" width="0.1524" layer="21" curve="-90"/>
              <wire x1="2.159" y1="-1.905" x2="2.413" y2="-1.651" width="0.1524" layer="21" curve="90"/>
              <wire x1="-2.413" y1="-1.651" x2="-2.159" y2="-1.905" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0" x2="0.381" y2="0" width="0.1524" layer="51"/>
              <wire x1="0.381" y1="0" x2="0.254" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="0.762" width="0.254" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0.762" x2="-0.254" y2="0" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.381" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="0" x2="-0.762" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.286" y="2.159" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-2.286" y="-3.429" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-050X050">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 5 x 5 mm</description>
              <wire x1="-2.159" y1="2.286" x2="2.159" y2="2.286" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-2.286" x2="-2.159" y2="-2.286" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="2.032" x2="2.413" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="2.032" x2="-2.413" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="2.286" x2="2.413" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.413" y1="2.032" x2="-2.159" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="2.159" y1="-2.286" x2="2.413" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-2.413" y1="-2.032" x2="-2.159" y2="-2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0" x2="0.381" y2="0" width="0.1524" layer="51"/>
              <wire x1="0.381" y1="0" x2="0.254" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="0.762" width="0.254" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0.762" x2="-0.254" y2="0" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.381" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="0" x2="-0.762" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.286" y="2.54" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-2.286" y="-3.81" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-060X050">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 2.5 mm, outline 6 x 5 mm</description>
              <wire x1="-2.159" y1="2.794" x2="2.159" y2="2.794" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="-2.794" x2="-2.159" y2="-2.794" width="0.1524" layer="21"/>
              <wire x1="2.413" y1="2.54" x2="2.413" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="-2.413" y1="2.54" x2="-2.413" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="2.159" y1="2.794" x2="2.413" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.413" y1="2.54" x2="-2.159" y2="2.794" width="0.1524" layer="21" curve="-90"/>
              <wire x1="2.159" y1="-2.794" x2="2.413" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-2.413" y1="-2.54" x2="-2.159" y2="-2.794" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="0" x2="0.381" y2="0" width="0.1524" layer="51"/>
              <wire x1="0.381" y1="0" x2="0.254" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="0.762" width="0.254" layer="21"/>
              <wire x1="0.254" y1="0" x2="0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0.762" x2="-0.254" y2="0" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.254" y2="-0.762" width="0.254" layer="21"/>
              <wire x1="-0.254" y1="0" x2="-0.381" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.381" y1="0" x2="-0.762" y2="0" width="0.1524" layer="51"/>
              <pad name="1" x="-1.27" y="0" drill="0.8"/>
              <pad name="2" x="1.27" y="0" drill="0.8"/>
              <text x="-2.286" y="3.048" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-2.032" y="-2.413" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-025X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 2.5 x 7.5 mm</description>
              <wire x1="0.3048" y1="-0.635" x2="0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3048" y1="0" x2="0.3048" y2="0.635" width="0.3048" layer="21"/>
              <wire x1="0.3048" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.3302" y1="-0.635" x2="-0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3302" y1="0" x2="-0.3302" y2="0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3302" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-1.016" x2="3.683" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="1.27" x2="-3.429" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="1.016" x2="-3.683" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-1.27" x2="3.429" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-1.27" x2="-3.683" y2="-1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-3.429" y1="1.27" x2="-3.683" y2="1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="3.683" y1="1.016" x2="3.429" y2="1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="3.683" y1="-1.016" x2="3.429" y2="-1.27" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="2.54" y="0" drill="0.8" rot="R180"/>
              <pad name="2" x="-2.54" y="0" drill="0.8" rot="R180"/>
              <text x="4.064" y="1.27" size="0.6096" layer="25" rot="R270">&gt;NAME</text>
              <text x="-4.064" y="-1.27" size="0.6096" layer="27" rot="R90">&gt;VALUE</text>
            </package>
            <package name="C050-045X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 4.5 x 7.5 mm</description>
              <wire x1="-0.3048" y1="0.635" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.3302" y1="0.635" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="2.032" x2="-3.683" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-2.286" x2="3.429" y2="-2.286" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-2.032" x2="3.683" y2="2.032" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.286" x2="-3.429" y2="2.286" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.286" x2="3.683" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-2.286" x2="3.683" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-2.032" x2="-3.429" y2="-2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="2.032" x2="-3.429" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.556" y="2.667" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.556" y="-3.81" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-030X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 3 x 7.5 mm</description>
              <wire x1="-0.3048" y1="0.635" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.3302" y1="0.635" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="1.27" x2="-3.683" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-1.524" x2="3.429" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-1.27" x2="3.683" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="1.524" x2="-3.429" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="1.524" x2="3.683" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-1.524" x2="3.683" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-1.27" x2="-3.429" y2="-1.524" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="1.27" x2="-3.429" y2="1.524" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.556" y="1.905" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.556" y="-3.048" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-050X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 5 x 7.5 mm</description>
              <wire x1="-0.3048" y1="0.635" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.3302" y1="0.635" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="2.286" x2="-3.683" y2="-2.286" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-2.54" x2="3.429" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-2.286" x2="3.683" y2="2.286" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.54" x2="-3.429" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.54" x2="3.683" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-2.54" x2="3.683" y2="-2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-2.286" x2="-3.429" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="2.286" x2="-3.429" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.429" y="2.921" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.175" y="-2.159" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-055X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 5.5 x 7.5 mm</description>
              <wire x1="-0.3048" y1="0.635" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.3302" y1="0.635" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="2.54" x2="-3.683" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-2.794" x2="3.429" y2="-2.794" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-2.54" x2="3.683" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.794" x2="-3.429" y2="2.794" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="2.794" x2="3.683" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-2.794" x2="3.683" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-2.54" x2="-3.429" y2="-2.794" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="2.54" x2="-3.429" y2="2.794" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.429" y="3.175" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.302" y="-2.286" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-075X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 7.5 x 7.5 mm</description>
              <wire x1="-1.524" y1="0" x2="-0.4572" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.4572" y1="0" x2="-0.4572" y2="0.762" width="0.4064" layer="21"/>
              <wire x1="-0.4572" y1="0" x2="-0.4572" y2="-0.762" width="0.4064" layer="21"/>
              <wire x1="0.4318" y1="0.762" x2="0.4318" y2="0" width="0.4064" layer="21"/>
              <wire x1="0.4318" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.4318" y1="0" x2="0.4318" y2="-0.762" width="0.4064" layer="21"/>
              <wire x1="-3.683" y1="3.429" x2="-3.683" y2="-3.429" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-3.683" x2="3.429" y2="-3.683" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-3.429" x2="3.683" y2="3.429" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="3.683" x2="-3.429" y2="3.683" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="3.683" x2="3.683" y2="3.429" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-3.683" x2="3.683" y2="-3.429" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-3.429" x2="-3.429" y2="-3.683" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="3.429" x2="-3.429" y2="3.683" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.429" y="4.064" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.175" y="-2.921" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C075-032X103">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 7.5 mm, outline 3.2 x 10.3 mm</description>
              <wire x1="4.826" y1="1.524" x2="-4.826" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="-1.27" width="0.1524" layer="21"/>
              <wire x1="-4.826" y1="-1.524" x2="4.826" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="5.08" y1="-1.27" x2="5.08" y2="1.27" width="0.1524" layer="21"/>
              <wire x1="4.826" y1="1.524" x2="5.08" y2="1.27" width="0.1524" layer="21" curve="-90"/>
              <wire x1="4.826" y1="-1.524" x2="5.08" y2="-1.27" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.08" y1="-1.27" x2="-4.826" y2="-1.524" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.08" y1="1.27" x2="-4.826" y2="1.524" width="0.1524" layer="21" curve="-90"/>
              <wire x1="0.508" y1="0" x2="2.54" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.54" y1="0" x2="-0.508" y2="0" width="0.1524" layer="21"/>
              <wire x1="-0.508" y1="0.889" x2="-0.508" y2="0" width="0.4064" layer="21"/>
              <wire x1="-0.508" y1="0" x2="-0.508" y2="-0.889" width="0.4064" layer="21"/>
              <wire x1="0.508" y1="0.889" x2="0.508" y2="0" width="0.4064" layer="21"/>
              <wire x1="0.508" y1="0" x2="0.508" y2="-0.889" width="0.4064" layer="21"/>
              <pad name="1" x="-3.81" y="0" drill="0.9"/>
              <pad name="2" x="3.81" y="0" drill="0.9"/>
              <text x="-4.826" y="1.905" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-4.826" y="-3.048" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C075-042X103">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 7.5 mm, outline 4.2 x 10.3 mm</description>
              <wire x1="4.826" y1="2.032" x2="-4.826" y2="2.032" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="1.778" x2="-5.08" y2="-1.778" width="0.1524" layer="21"/>
              <wire x1="-4.826" y1="-2.032" x2="4.826" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="5.08" y1="-1.778" x2="5.08" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="4.826" y1="2.032" x2="5.08" y2="1.778" width="0.1524" layer="21" curve="-90"/>
              <wire x1="4.826" y1="-2.032" x2="5.08" y2="-1.778" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.08" y1="-1.778" x2="-4.826" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.08" y1="1.778" x2="-4.826" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="0" x2="2.667" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.667" y1="0" x2="-2.159" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.159" y1="1.27" x2="-2.159" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.159" y1="0" x2="-2.159" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.4064" layer="21"/>
              <pad name="1" x="-3.81" y="0" drill="0.9"/>
              <pad name="2" x="3.81" y="0" drill="0.9"/>
              <text x="-4.699" y="2.413" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-0.635" y="-1.651" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C075-052X106">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 7.5 mm, outline 5.2 x 10.6 mm</description>
              <wire x1="4.953" y1="2.54" x2="-4.953" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="-5.207" y1="2.286" x2="-5.207" y2="-2.286" width="0.1524" layer="21"/>
              <wire x1="-4.953" y1="-2.54" x2="4.953" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="5.207" y1="-2.286" x2="5.207" y2="2.286" width="0.1524" layer="21"/>
              <wire x1="4.953" y1="2.54" x2="5.207" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="4.953" y1="-2.54" x2="5.207" y2="-2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.207" y1="-2.286" x2="-4.953" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.207" y1="2.286" x2="-4.953" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="0" x2="2.667" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.667" y1="0" x2="-2.159" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.159" y1="1.27" x2="-2.159" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.159" y1="0" x2="-2.159" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.4064" layer="21"/>
              <pad name="1" x="-3.81" y="0" drill="0.9"/>
              <pad name="2" x="3.81" y="0" drill="0.9"/>
              <text x="-4.826" y="2.921" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-0.635" y="-2.032" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C102-043X133">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 10.2 mm, outline 4.3 x 13.3 mm</description>
              <wire x1="-3.175" y1="1.27" x2="-3.175" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.286" y1="1.27" x2="-2.286" y2="0" width="0.4064" layer="21"/>
              <wire x1="3.81" y1="0" x2="-2.286" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="0" x2="-2.286" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-3.81" y1="0" x2="-3.175" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.175" y1="0" x2="-3.175" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-6.096" y1="2.032" x2="6.096" y2="2.032" width="0.1524" layer="21"/>
              <wire x1="6.604" y1="1.524" x2="6.604" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="-2.032" x2="-6.096" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="-6.604" y1="-1.524" x2="-6.604" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="2.032" x2="6.604" y2="1.524" width="0.1524" layer="21" curve="-90"/>
              <wire x1="6.096" y1="-2.032" x2="6.604" y2="-1.524" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="-1.524" x2="-6.096" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="1.524" x2="-6.096" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="1"/>
              <pad name="2" x="5.08" y="0" drill="1"/>
              <text x="-6.096" y="2.413" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.524" y="-1.651" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C102-054X133">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 10.2 mm, outline 5.4 x 13.3 mm</description>
              <wire x1="-3.175" y1="1.27" x2="-3.175" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.286" y1="1.27" x2="-2.286" y2="0" width="0.4064" layer="21"/>
              <wire x1="3.81" y1="0" x2="-2.286" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="0" x2="-2.286" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-3.81" y1="0" x2="-3.175" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.175" y1="0" x2="-3.175" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-6.096" y1="2.54" x2="6.096" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="6.604" y1="2.032" x2="6.604" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="-2.54" x2="-6.096" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="-6.604" y1="-2.032" x2="-6.604" y2="2.032" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="2.54" x2="6.604" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <wire x1="6.096" y1="-2.54" x2="6.604" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="-2.032" x2="-6.096" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="2.032" x2="-6.096" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="1"/>
              <pad name="2" x="5.08" y="0" drill="1"/>
              <text x="-6.096" y="2.921" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.524" y="-1.905" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C102-064X133">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 10.2 mm, outline 6.4 x 13.3 mm</description>
              <wire x1="-3.175" y1="1.27" x2="-3.175" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.286" y1="1.27" x2="-2.286" y2="0" width="0.4064" layer="21"/>
              <wire x1="3.81" y1="0" x2="-2.286" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="0" x2="-2.286" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-3.81" y1="0" x2="-3.175" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.175" y1="0" x2="-3.175" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-6.096" y1="3.048" x2="6.096" y2="3.048" width="0.1524" layer="21"/>
              <wire x1="6.604" y1="2.54" x2="6.604" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="-3.048" x2="-6.096" y2="-3.048" width="0.1524" layer="21"/>
              <wire x1="-6.604" y1="-2.54" x2="-6.604" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="6.096" y1="3.048" x2="6.604" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="6.096" y1="-3.048" x2="6.604" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="-2.54" x2="-6.096" y2="-3.048" width="0.1524" layer="21" curve="90"/>
              <wire x1="-6.604" y1="2.54" x2="-6.096" y2="3.048" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="1"/>
              <pad name="2" x="5.08" y="0" drill="1"/>
              <text x="-6.096" y="3.429" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.524" y="-2.032" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C150-054X183">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 15 mm, outline 5.4 x 18.3 mm</description>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="9.017" y1="2.032" x2="9.017" y2="-2.032" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="-2.54" x2="-8.509" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="-9.017" y1="-2.032" x2="-9.017" y2="2.032" width="0.1524" layer="21"/>
              <wire x1="-8.509" y1="2.54" x2="8.509" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="2.54" x2="9.017" y2="2.032" width="0.1524" layer="21" curve="-90"/>
              <wire x1="8.509" y1="-2.54" x2="9.017" y2="-2.032" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="-2.032" x2="-8.509" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="2.032" x2="-8.509" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.493" y="0" drill="1"/>
              <pad name="2" x="7.493" y="0" drill="1"/>
              <text x="-8.382" y="2.921" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.429" y="-2.032" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C150-064X183">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 15 mm, outline 6.4 x 18.3 mm</description>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="9.017" y1="2.54" x2="9.017" y2="-2.54" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="-3.048" x2="-8.509" y2="-3.048" width="0.1524" layer="21"/>
              <wire x1="-9.017" y1="-2.54" x2="-9.017" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="-8.509" y1="3.048" x2="8.509" y2="3.048" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="3.048" x2="9.017" y2="2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="8.509" y1="-3.048" x2="9.017" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="-2.54" x2="-8.509" y2="-3.048" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="2.54" x2="-8.509" y2="3.048" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.493" y="0" drill="1"/>
              <pad name="2" x="7.493" y="0" drill="1"/>
              <text x="-8.509" y="3.429" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.429" y="-2.032" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C150-072X183">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 15 mm, outline 7.2 x 18.3 mm</description>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="9.017" y1="3.048" x2="9.017" y2="-3.048" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="-3.556" x2="-8.509" y2="-3.556" width="0.1524" layer="21"/>
              <wire x1="-9.017" y1="-3.048" x2="-9.017" y2="3.048" width="0.1524" layer="21"/>
              <wire x1="-8.509" y1="3.556" x2="8.509" y2="3.556" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="3.556" x2="9.017" y2="3.048" width="0.1524" layer="21" curve="-90"/>
              <wire x1="8.509" y1="-3.556" x2="9.017" y2="-3.048" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="-3.048" x2="-8.509" y2="-3.556" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="3.048" x2="-8.509" y2="3.556" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.493" y="0" drill="1"/>
              <pad name="2" x="7.493" y="0" drill="1"/>
              <text x="-8.509" y="3.937" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.429" y="-2.286" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C150-084X183">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 15 mm, outline 8.4 x 18.3 mm</description>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="9.017" y1="3.556" x2="9.017" y2="-3.556" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="-4.064" x2="-8.509" y2="-4.064" width="0.1524" layer="21"/>
              <wire x1="-9.017" y1="-3.556" x2="-9.017" y2="3.556" width="0.1524" layer="21"/>
              <wire x1="-8.509" y1="4.064" x2="8.509" y2="4.064" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="4.064" x2="9.017" y2="3.556" width="0.1524" layer="21" curve="-90"/>
              <wire x1="8.509" y1="-4.064" x2="9.017" y2="-3.556" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="-3.556" x2="-8.509" y2="-4.064" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="3.556" x2="-8.509" y2="4.064" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.493" y="0" drill="1"/>
              <pad name="2" x="7.493" y="0" drill="1"/>
              <text x="-8.509" y="4.445" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.429" y="-2.54" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C150-091X182">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 15 mm, outline 9.1 x 18.2 mm</description>
              <wire x1="-5.08" y1="1.27" x2="-5.08" y2="0" width="0.4064" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-5.08" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="1.27" x2="-4.191" y2="0" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="-4.191" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-4.191" y1="0" x2="6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="-5.08" y1="0" x2="-6.096" y2="0" width="0.1524" layer="21"/>
              <wire x1="9.017" y1="3.937" x2="9.017" y2="-3.937" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="-4.445" x2="-8.509" y2="-4.445" width="0.1524" layer="21"/>
              <wire x1="-9.017" y1="-3.937" x2="-9.017" y2="3.937" width="0.1524" layer="21"/>
              <wire x1="-8.509" y1="4.445" x2="8.509" y2="4.445" width="0.1524" layer="21"/>
              <wire x1="8.509" y1="4.445" x2="9.017" y2="3.937" width="0.1524" layer="21" curve="-90"/>
              <wire x1="8.509" y1="-4.445" x2="9.017" y2="-3.937" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="-3.937" x2="-8.509" y2="-4.445" width="0.1524" layer="21" curve="90"/>
              <wire x1="-9.017" y1="3.937" x2="-8.509" y2="4.445" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-7.493" y="0" drill="1"/>
              <pad name="2" x="7.493" y="0" drill="1"/>
              <text x="-8.509" y="4.826" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.429" y="-2.54" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C050-035X075">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 5 mm, outline 3.5 x 7.5 mm</description>
              <wire x1="-0.3048" y1="0.635" x2="-0.3048" y2="0" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-0.3048" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="-0.3048" y1="0" x2="-1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="0.3302" y1="0.635" x2="0.3302" y2="0" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="0.3302" y2="-0.635" width="0.3048" layer="21"/>
              <wire x1="0.3302" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
              <wire x1="-3.683" y1="1.524" x2="-3.683" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="-3.429" y1="-1.778" x2="3.429" y2="-1.778" width="0.1524" layer="21"/>
              <wire x1="3.683" y1="-1.524" x2="3.683" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="1.778" x2="-3.429" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="3.429" y1="1.778" x2="3.683" y2="1.524" width="0.1524" layer="21" curve="-90"/>
              <wire x1="3.429" y1="-1.778" x2="3.683" y2="-1.524" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="-1.524" x2="-3.429" y2="-1.778" width="0.1524" layer="21" curve="90"/>
              <wire x1="-3.683" y1="1.524" x2="-3.429" y2="1.778" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-2.54" y="0" drill="0.8"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="-3.556" y="2.159" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-3.556" y="-3.429" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C075-063X106">
              <description>&lt;b&gt;CAPACITOR&lt;/b&gt;&lt;p&gt;
 grid 7.5 mm, outline 6.3 x 10.6 mm</description>
              <wire x1="4.953" y1="3.048" x2="-4.953" y2="3.048" width="0.1524" layer="21"/>
              <wire x1="-5.207" y1="2.794" x2="-5.207" y2="-2.794" width="0.1524" layer="21"/>
              <wire x1="-4.953" y1="-3.048" x2="4.953" y2="-3.048" width="0.1524" layer="21"/>
              <wire x1="5.207" y1="-2.794" x2="5.207" y2="2.794" width="0.1524" layer="21"/>
              <wire x1="4.953" y1="3.048" x2="5.207" y2="2.794" width="0.1524" layer="21" curve="-90"/>
              <wire x1="4.953" y1="-3.048" x2="5.207" y2="-2.794" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.207" y1="-2.794" x2="-4.953" y2="-3.048" width="0.1524" layer="21" curve="90"/>
              <wire x1="-5.207" y1="2.794" x2="-4.953" y2="3.048" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="0" x2="2.667" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.667" y1="0" x2="-2.159" y2="0" width="0.1524" layer="21"/>
              <wire x1="-2.159" y1="1.27" x2="-2.159" y2="0" width="0.4064" layer="21"/>
              <wire x1="-2.159" y1="0" x2="-2.159" y2="-1.27" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.4064" layer="21"/>
              <wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.4064" layer="21"/>
              <pad name="1" x="-3.81" y="0" drill="0.9"/>
              <pad name="2" x="3.81" y="0" drill="0.9"/>
              <text x="-4.826" y="3.429" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-0.635" y="-2.54" size="0.6096" layer="27">&gt;VALUE</text>
            </package>
            <package name="C025-020X036">
              <wire x1="-0.889" y1="0" x2="-0.127" y2="0.889" width="0.091440625" layer="21" curve="-90"/>
              <wire x1="2.667" y1="0.889" x2="-0.127" y2="0.889" width="0.091440625" layer="21"/>
              <wire x1="2.667" y1="0.889" x2="3.429" y2="0" width="0.091440625" layer="21" curve="-90"/>
              <wire x1="0.9652" y1="0.508" x2="0.9652" y2="-0.508" width="0.091440625" layer="21"/>
              <wire x1="2.54" y1="0" x2="1.6637" y2="0" width="0.0762" layer="51"/>
              <wire x1="0" y1="0" x2="0.889" y2="0" width="0.0762" layer="51"/>
              <pad name="2" x="2.54" y="0" drill="0.8"/>
              <text x="0.254" y="0.9652" size="0.6096" layer="25" align="bottom-right">&gt;NAME</text>
              <text x="3.556" y="0" size="0.6096" layer="27" align="center-left">&gt;VALUE</text>
              <pad name="1" x="0" y="0" drill="0.8" rot="R180"/>
              <wire x1="3.429" y1="0" x2="2.667" y2="-0.889" width="0.091440625" layer="21" curve="-90"/>
              <wire x1="-0.127" y1="-0.889" x2="2.667" y2="-0.889" width="0.091440625" layer="21"/>
              <wire x1="-0.127" y1="-0.889" x2="-0.889" y2="0" width="0.091440625" layer="21" curve="-90"/>
              <rectangle x1="0.4445" y1="-0.127" x2="1.5875" y2="0.127" layer="21" rot="R90"/>
              <rectangle x1="0.9525" y1="-0.127" x2="2.0955" y2="0.127" layer="21" rot="R270"/>
            </package>
            <package name="C050-025X060">
              <wire x1="0.635" y1="0.3048" x2="0" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0" y1="0.3048" x2="-0.635" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0" y1="0.3048" x2="0" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="-0.3302" x2="0" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-0.3302" x2="-0.635" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-0.3302" x2="0" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="0.762" y1="3.556" x2="-0.762" y2="3.556" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="3.302" x2="-1.016" y2="-3.302" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-3.556" x2="0.762" y2="-3.556" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-3.302" x2="1.016" y2="3.302" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-3.302" x2="0.762" y2="-3.556" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.016" y1="-3.302" x2="-0.762" y2="-3.556" width="0.1524" layer="21" curve="90"/>
              <wire x1="-0.762" y1="3.556" x2="-1.016" y2="3.302" width="0.1524" layer="21" curve="90"/>
              <wire x1="0.762" y1="3.556" x2="1.016" y2="3.302" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="2.54" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-2.54" drill="0.8" rot="R270"/>
              <text x="0" y="3.81" size="0.6096" layer="25" align="bottom-center">&gt;NAME</text>
              <text x="0" y="-3.81" size="0.6096" layer="27" align="top-center">&gt;VALUE</text>
            </package>
            <package name="C050-025X075/V">
              <wire x1="0.635" y1="0.3048" x2="0" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0" y1="0.3048" x2="-0.635" y2="0.3048" width="0.3048" layer="21"/>
              <wire x1="0" y1="0.3048" x2="0" y2="1.524" width="0.1524" layer="21"/>
              <wire x1="0.635" y1="-0.3302" x2="0" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-0.3302" x2="-0.635" y2="-0.3302" width="0.3048" layer="21"/>
              <wire x1="0" y1="-0.3302" x2="0" y2="-1.524" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="3.683" x2="-1.016" y2="3.683" width="0.1524" layer="21"/>
              <wire x1="-1.27" y1="3.429" x2="-1.27" y2="-3.429" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-3.683" x2="1.016" y2="-3.683" width="0.1524" layer="21"/>
              <wire x1="1.27" y1="-3.429" x2="1.27" y2="3.429" width="0.1524" layer="21"/>
              <wire x1="1.27" y1="-3.429" x2="1.016" y2="-3.683" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-1.27" y1="-3.429" x2="-1.016" y2="-3.683" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="3.683" x2="-1.27" y2="3.429" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="3.683" x2="1.27" y2="3.429" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="2.54" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-2.54" drill="0.8" rot="R270"/>
              <text x="0" y="4.064" size="0.6096" layer="25" align="bottom-center">&gt;NAME</text>
              <text x="0" y="-4.064" size="0.6096" layer="27" align="top-center">&gt;VALUE</text>
            </package>
            <package name="0204/10">
              <description>&lt;b&gt;Potentiometers&lt;/b&gt;&lt;p&gt;
 Beckman, Copal, Piher, Spectrol, Schukat&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="-5.08" y="0" drill="0.8"/>
              <pad name="2" x="5.08" y="0" drill="0.8"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
              <wire x1="5.08" y1="0" x2="2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-5.08" y1="0" x2="-2.921" y2="0" width="0.508" layer="51"/>
              <wire x1="-2.54" y1="0.762" x2="-2.286" y2="1.016" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.286" y2="-1.016" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="-1.016" x2="2.54" y2="-0.762" width="0.1524" layer="21" curve="90"/>
              <wire x1="2.286" y1="1.016" x2="2.54" y2="0.762" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-2.54" y1="-0.762" x2="-2.54" y2="0.762" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="1.016" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="0.889" x2="-1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="-2.286" y1="-1.016" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="-1.778" y1="-0.889" x2="-1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="0.889" x2="-1.778" y2="0.889" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="1.778" y1="-0.889" x2="-1.778" y2="-0.889" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="1.016" x2="1.905" y2="1.016" width="0.1524" layer="21"/>
              <wire x1="2.286" y1="-1.016" x2="1.905" y2="-1.016" width="0.1524" layer="21"/>
              <wire x1="2.54" y1="-0.762" x2="2.54" y2="0.762" width="0.1524" layer="21"/>
              <text x="-2.54" y="1.2954" size="0.6096" layer="25">&gt;NAME</text>
              <text x="-1.6256" y="-0.4826" size="0.6096" layer="27">&gt;VALUE</text>
              <rectangle x1="2.54" y1="-0.254" x2="2.921" y2="0.254" layer="21"/>
              <rectangle x1="-2.921" y1="-0.254" x2="-2.54" y2="0.254" layer="21"/>
            </package>
            <package name="0204/10-V">
              <description>&lt;b&gt;Potentiometers&lt;/b&gt;&lt;p&gt;
 Beckman, Copal, Piher, Spectrol, Schukat&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <pad name="1" x="0" y="5.08" drill="0.8" rot="R270"/>
              <pad name="2" x="0" y="-5.08" drill="0.8" rot="R270"/>
              <text x="-0.635" y="3.175" size="0.6096" layer="25" align="bottom-right">&gt;NAME</text>
              <text x="0" y="0" size="0.6096" layer="27" rot="R270" align="center">&gt;VALUE</text>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
              <wire x1="0" y1="-5.08" x2="0" y2="-2.921" width="0.508" layer="51"/>
              <wire x1="0" y1="5.08" x2="0" y2="2.921" width="0.508" layer="51"/>
              <wire x1="0.762" y1="2.54" x2="1.016" y2="2.286" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="-1.016" y2="2.286" width="0.1524" layer="21" curve="90"/>
              <wire x1="-1.016" y1="-2.286" x2="-0.762" y2="-2.54" width="0.1524" layer="21" curve="90"/>
              <wire x1="1.016" y1="-2.286" x2="0.762" y2="-2.54" width="0.1524" layer="21" curve="-90"/>
              <wire x1="-0.762" y1="2.54" x2="0.762" y2="2.54" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="2.286" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="1.778" x2="1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="2.286" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="1.778" x2="-1.016" y2="1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="0.889" y1="-1.778" x2="0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.889" y1="-1.778" x2="-0.889" y2="1.778" width="0.1524" layer="21"/>
              <wire x1="1.016" y1="-2.286" x2="1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-1.016" y1="-2.286" x2="-1.016" y2="-1.905" width="0.1524" layer="21"/>
              <wire x1="-0.762" y1="-2.54" x2="0.762" y2="-2.54" width="0.1524" layer="21"/>
              <rectangle x1="-0.1905" y1="-2.9845" x2="0.1905" y2="-2.4765" layer="21" rot="R270"/>
              <rectangle x1="-0.1905" y1="2.4765" x2="0.1905" y2="2.9845" layer="21" rot="R270"/>
            </package>
          </packages>
          <symbols>
            <symbol name="C-H">
              <text x="-1.27" y="1.27" size="1.27" layer="95" align="bottom-right">&gt;NAME</text>
              <text x="1.27" y="1.27" size="1.27" layer="96">&gt;VALUE</text>
              <pin name="1" x="-2.54" y="0" visible="off" length="point" direction="pas" swaplevel="1"/>
              <pin name="2" x="2.54" y="0" visible="off" length="point" direction="pas" swaplevel="1" rot="R180"/>
              <wire x1="2.54" y1="0" x2="0.762" y2="0" width="0.1524" layer="94"/>
              <wire x1="-2.54" y1="0" x2="-0.762" y2="0" width="0.1524" layer="94"/>
              <rectangle x1="-1.27" y1="-0.254" x2="2.286" y2="0.254" layer="94" rot="R90"/>
              <rectangle x1="-2.286" y1="-0.254" x2="1.27" y2="0.254" layer="94" rot="R90"/>
            </symbol>
            <symbol name="C">
              <text x="-1.27" y="1.27" size="1.27" layer="95" align="bottom-right">&gt;NAME</text>
              <text x="-1.27" y="-1.27" size="1.27" layer="96" align="top-right">&gt;VALUE</text>
              <pin name="1" x="0" y="2.54" visible="off" length="point" direction="pas" swaplevel="1" rot="R270"/>
              <pin name="2" x="0" y="-2.54" visible="off" length="point" direction="pas" swaplevel="1" rot="R90"/>
              <wire x1="0" y1="-2.54" x2="0" y2="-0.762" width="0.1524" layer="94"/>
              <wire x1="0" y1="2.54" x2="0" y2="0.762" width="0.1524" layer="94"/>
              <rectangle x1="-1.778" y1="-0.762" x2="1.778" y2="-0.254" layer="94"/>
              <rectangle x1="-1.778" y1="0.254" x2="1.778" y2="0.762" layer="94"/>
            </symbol>
          </symbols>
          <devicesets>
            <deviceset name="C-H" prefix="C" uservalue="yes">
              <gates>
                <gate name="G$1" symbol="C-H" x="0" y="0"/>
              </gates>
              <devices>
                <device name="025-015X024" package="C025-015X024">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X044" package="C050-020X044">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-020X044" package="C075-020X044">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-020X044/V" package="C075-020X044/V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X044/V" package="C050-020X044/V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-024X044" package="C050-024X044">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-024X044/V" package="C050-024X044/V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X036" package="C050-020X036">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X036/V" package="C050-020X036/V">
                  <connects>
                    <connect gate="G$1" pin="1" pad="1"/>
                    <connect gate="G$1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
            <deviceset name="C" prefix="C" uservalue="yes">
              <description>&lt;B&gt;CAPACITOR&lt;/B&gt;, European symbol</description>
              <gates>
                <gate name="1" symbol="C" x="0" y="0"/>
              </gates>
              <devices>
                <device name="025-024X044" package="C025-024X044">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-025X050" package="C025-025X050">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-030X050" package="C025-030X050">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-040X050" package="C025-040X050">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-050X050" package="C025-050X050">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-060X050" package="C025-060X050">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-024X044" package="C050-024X044">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-025X075" package="C050-025X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-045X075" package="C050-045X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-030X075" package="C050-030X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-050X075" package="C050-050X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-055X075" package="C050-055X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-075X075" package="C050-075X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-032X103" package="C075-032X103">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-042X103" package="C075-042X103">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-052X106" package="C075-052X106">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="102-043X133" package="C102-043X133">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="102-054X133" package="C102-054X133">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="102-064X133" package="C102-064X133">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="150-054X183" package="C150-054X183">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="150-064X183" package="C150-064X183">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="150-072X183" package="C150-072X183">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="150-084X183" package="C150-084X183">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="150-091X182" package="C150-091X182">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-035X075" package="C050-035X075">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-063X106" package="C075-063X106">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-020X036" package="C025-020X036">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X036" package="C050-020X036">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-015X024" package="C025-015X024">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="025-025X060" package="C050-025X060">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-024X044/V" package="C050-024X044/V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-025X075/V" package="C050-025X075/V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X036/V" package="C050-020X036/V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X044/V" package="C050-020X044/V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="050-020X044" package="C050-020X044">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10" package="0204/10">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="0204/10-V" package="0204/10-V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-020X044" package="C075-020X044">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
                <device name="075-020X044/V" package="C075-020X044/V">
                  <connects>
                    <connect gate="1" pin="1" pad="1"/>
                    <connect gate="1" pin="2" pad="2"/>
                  </connects>
                  <technologies>
                    <technology name=""/>
                  </technologies>
                </device>
              </devices>
            </deviceset>
          </devicesets>
        </library>
      </libraries>
      <attributes>
</attributes>
      <variantdefs>
</variantdefs>
      <classes>
        <class number="0" name="default" width="0" drill="0">
</class>
      </classes>
      <parts>
        <part name="J1" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="J2" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="S1" library="pad" deviceset="GND" device=""/>
        <part name="S2" library="pad" deviceset="GND" device=""/>
        <part name="R19" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="R18" library="r" deviceset="R-H" device="0204/10" value="33k"/>
        <part name="R20" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="R34" library="r" deviceset="R-H" device="0204/10" value="3.3M"/>
        <part name="R30" library="r" deviceset="POT" device="" value="100k"/>
        <part name="R31" library="r" deviceset="POT" device="" value="100k"/>
        <part name="U$1" library="pad" deviceset="VCC" device=""/>
        <part name="U$2" library="pad" deviceset="VCC" device=""/>
        <part name="U$3" library="pad" deviceset="VEE" device=""/>
        <part name="U$4" library="pad" deviceset="VEE" device=""/>
        <part name="R29" library="r" deviceset="POT" device="" value="100k"/>
        <part name="S3" library="pad" deviceset="GND" device=""/>
        <part name="S4" library="pad" deviceset="GND" device=""/>
        <part name="R6" library="r" deviceset="POT" device="-V" value="100"/>
        <part name="R7" library="r" deviceset="R" device="0204/10-V" value="390"/>
        <part name="S5" library="pad" deviceset="GND" device=""/>
        <part name="T1" library="t" deviceset="2N3906" device=""/>
        <part name="T2" library="t" deviceset="2N3906" device=""/>
        <part name="C5" library="c" deviceset="C-H" device="050-020X036" value="1nF"/>
        <part name="IC4" library="ic" deviceset="TL071" device="P"/>
        <part name="R12" library="r" deviceset="R" device="0204/10-V" value="5.6k"/>
        <part name="S6" library="pad" deviceset="GND" device=""/>
        <part name="S7" library="pad" deviceset="GND" device=""/>
        <part name="R21" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="U$5" library="pad" deviceset="VEE" device=""/>
        <part name="U$6" library="pad" deviceset="VEE" device=""/>
        <part name="U$7" library="pad" deviceset="VCC" device=""/>
        <part name="U$8" library="pad" deviceset="VEE" device=""/>
        <part name="U$9" library="pad" deviceset="VCC" device=""/>
        <part name="J3" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="C14" library="c" deviceset="C" device="050-020X044/V" value="0.22uF"/>
        <part name="R32" library="r" deviceset="POT" device=""/>
        <part name="R24" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="R17" library="r" deviceset="R-H" device="0204/10" value="27k"/>
        <part name="R23" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="C1" library="c" deviceset="C-H" device="050-020X036" value="10pF"/>
        <part name="R13" library="r" deviceset="R-H" device="0204/10" value="11k"/>
        <part name="R22" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="IC2" library="ic" deviceset="LM13600" device=""/>
        <part name="R1" library="r" deviceset="R" device="0204/10-V" value="22"/>
        <part name="S8" library="pad" deviceset="GND" device=""/>
        <part name="R8" library="r" deviceset="R" device="0204/10-V" value="1k"/>
        <part name="J4" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="S9" library="pad" deviceset="GND" device=""/>
        <part name="R3" library="r" deviceset="R" device="0204/10-V" value="22"/>
        <part name="R33" library="r" deviceset="POT" device="" value="100k"/>
        <part name="R15" library="r" deviceset="R" device="0204/10-V" value="15k"/>
        <part name="C3" library="c" deviceset="C-H" device="050-020X036" value="330pF"/>
        <part name="R9" library="r" deviceset="R" device="0204/10-V" value="1k"/>
        <part name="S10" library="pad" deviceset="GND" device=""/>
        <part name="J5" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="S11" library="pad" deviceset="GND" device=""/>
        <part name="R14" library="r" deviceset="R-H" device="0204/10" value="11k"/>
        <part name="C2" library="c" deviceset="C-H" device="050-020X036" value="10pF"/>
        <part name="S12" library="pad" deviceset="GND" device=""/>
        <part name="S13" library="pad" deviceset="GND" device=""/>
        <part name="R2" library="r" deviceset="R" device="0204/10-V" value="22"/>
        <part name="R25" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="R27" library="r" deviceset="POT" device="-V" value="100"/>
        <part name="S14" library="pad" deviceset="GND" device=""/>
        <part name="U$10" library="pad" deviceset="VEE" device=""/>
        <part name="U$11" library="pad" deviceset="VCC" device=""/>
        <part name="R4" library="r" deviceset="R" device="0204/10-V" value="22"/>
        <part name="R26" library="r" deviceset="R-H" device="0204/10" value="100k"/>
        <part name="R28" library="r" deviceset="POT" device="-V" value="100"/>
        <part name="S15" library="pad" deviceset="GND" device=""/>
        <part name="U$12" library="pad" deviceset="VEE" device=""/>
        <part name="U$13" library="pad" deviceset="VCC" device=""/>
        <part name="R16" library="r" deviceset="R" device="0204/10-V" value="15k"/>
        <part name="C4" library="c" deviceset="C-H" device="050-020X036" value="330pF"/>
        <part name="R10" library="r" deviceset="R" device="0204/10-V" value="1k"/>
        <part name="S16" library="pad" deviceset="GND" device=""/>
        <part name="J6" library="pad" deviceset="SPAD+-" device="-+"/>
        <part name="R5" library="r" deviceset="R-H" device="0204/10" value="47"/>
        <part name="S17" library="pad" deviceset="GND" device=""/>
        <part name="S18" library="pad" deviceset="GND" device=""/>
        <part name="S19" library="pad" deviceset="GND" device=""/>
        <part name="S20" library="pad" deviceset="GND" device=""/>
        <part name="IC1" library="ic" deviceset="TL072" device="P"/>
        <part name="IC5" library="ic" deviceset="TL072" device="P"/>
      </parts>
      <sheets>
        <sheet>
          <plain>
</plain>
          <instances>
            <instance part="J1" gate="-" x="-33.02" y="78.74"/>
            <instance part="J1" gate="+" x="-33.02" y="83.82"/>
            <instance part="J2" gate="+" x="-33.02" y="68.58"/>
            <instance part="J2" gate="-" x="-33.02" y="63.5"/>
            <instance part="S1" gate="1" x="-27.94" y="73.66"/>
            <instance part="S2" gate="1" x="-27.94" y="58.42"/>
            <instance part="R19" gate="G$1" x="-7.62" y="83.82"/>
            <instance part="R18" gate="G$1" x="-7.62" y="60.96"/>
            <instance part="R20" gate="G$1" x="-7.62" y="38.1"/>
            <instance part="R34" gate="G$1" x="-7.62" y="10.16"/>
            <instance part="R30" gate="1" x="-35.56" y="38.1" rot="MR180"/>
            <instance part="R31" gate="1" x="-35.56" y="10.16" rot="MR180"/>
            <instance part="U$1" gate="G$1" x="-35.56" y="45.72"/>
            <instance part="U$2" gate="G$1" x="-35.56" y="17.78"/>
            <instance part="U$3" gate="G$1" x="-35.56" y="30.48"/>
            <instance part="U$4" gate="G$1" x="-35.56" y="2.54"/>
            <instance part="R29" gate="1" x="-20.32" y="60.96"/>
            <instance part="S3" gate="1" x="-20.32" y="50.8"/>
            <instance part="S4" gate="1" x="7.62" y="71.12"/>
            <instance part="R6" gate="1" x="33.02" y="73.66" rot="MR180"/>
            <instance part="R7" gate="G$1" x="33.02" y="60.96" rot="MR0"/>
            <instance part="S5" gate="1" x="33.02" y="50.8"/>
            <instance part="T1" gate="G$1" x="38.1" y="38.1" rot="MR270"/>
            <instance part="T2" gate="G$1" x="58.42" y="38.1" rot="MR270"/>
            <instance part="C5" gate="G$1" x="38.1" y="17.78"/>
            <instance part="IC4" gate="A" x="38.1" y="0" rot="MR180"/>
            <instance part="R12" gate="G$1" x="48.26" y="25.4"/>
            <instance part="S6" gate="1" x="22.86" y="38.1"/>
            <instance part="S7" gate="1" x="27.94" y="-7.62"/>
            <instance part="R21" gate="G$1" x="20.32" y="2.54"/>
            <instance part="U$5" gate="G$1" x="10.16" y="2.54" rot="R270"/>
            <instance part="U$6" gate="G$1" x="20.32" y="73.66"/>
            <instance part="U$7" gate="G$1" x="20.32" y="88.9"/>
            <instance part="U$8" gate="G$1" x="38.1" y="-10.16"/>
            <instance part="U$9" gate="G$1" x="38.1" y="10.16"/>
            <instance part="J3" gate="-" x="88.9" y="66.04"/>
            <instance part="J3" gate="+" x="88.9" y="71.12"/>
            <instance part="C14" gate="1" x="101.6" y="60.96" rot="MR0"/>
            <instance part="R32" gate="1" x="101.6" y="50.8" rot="MR180"/>
            <instance part="R24" gate="G$1" x="129.54" y="58.42"/>
            <instance part="R17" gate="G$1" x="129.54" y="71.12"/>
            <instance part="R23" gate="G$1" x="129.54" y="83.82"/>
            <instance part="C1" gate="G$1" x="147.32" y="58.42"/>
            <instance part="R13" gate="G$1" x="147.32" y="48.26"/>
            <instance part="R22" gate="G$1" x="111.76" y="50.8"/>
            <instance part="IC2" gate="A" x="175.26" y="43.18" rot="MR180"/>
            <instance part="R1" gate="G$1" x="154.94" y="40.64"/>
            <instance part="S8" gate="1" x="154.94" y="30.48"/>
            <instance part="R8" gate="G$1" x="139.7" y="40.64" rot="MR0"/>
            <instance part="J4" gate="+" x="137.16" y="33.02"/>
            <instance part="J4" gate="-" x="137.16" y="27.94"/>
            <instance part="S9" gate="1" x="139.7" y="22.86"/>
            <instance part="R3" gate="G$1" x="228.6" y="33.02"/>
            <instance part="R33" gate="1" x="213.36" y="71.12" rot="MR0"/>
            <instance part="R15" gate="G$1" x="185.42" y="55.88"/>
            <instance part="C3" gate="G$1" x="203.2" y="53.34"/>
            <instance part="R9" gate="G$1" x="213.36" y="33.02" rot="MR0"/>
            <instance part="S10" gate="1" x="193.04" y="33.02"/>
            <instance part="J5" gate="+" x="210.82" y="25.4"/>
            <instance part="J5" gate="-" x="210.82" y="20.32"/>
            <instance part="S11" gate="1" x="213.36" y="15.24"/>
            <instance part="R14" gate="G$1" x="220.98" y="40.64"/>
            <instance part="C2" gate="G$1" x="220.98" y="53.34"/>
            <instance part="S12" gate="1" x="228.6" y="22.86"/>
            <instance part="IC2" gate="B" x="248.92" y="35.56" rot="MR180"/>
            <instance part="S13" gate="1" x="266.7" y="25.4"/>
            <instance part="R2" gate="G$1" x="162.56" y="0"/>
            <instance part="R25" gate="G$1" x="154.94" y="7.62"/>
            <instance part="R27" gate="1" x="144.78" y="7.62" rot="MR180"/>
            <instance part="S14" gate="1" x="162.56" y="-10.16"/>
            <instance part="U$10" gate="G$1" x="144.78" y="0"/>
            <instance part="U$11" gate="G$1" x="144.78" y="15.24"/>
            <instance part="R4" gate="G$1" x="236.22" y="-7.62"/>
            <instance part="R26" gate="G$1" x="228.6" y="0"/>
            <instance part="R28" gate="1" x="218.44" y="0" rot="MR180"/>
            <instance part="S15" gate="1" x="236.22" y="-17.78"/>
            <instance part="U$12" gate="G$1" x="218.44" y="-7.62"/>
            <instance part="U$13" gate="G$1" x="218.44" y="7.62"/>
            <instance part="R16" gate="G$1" x="259.08" y="48.26"/>
            <instance part="C4" gate="G$1" x="276.86" y="45.72"/>
            <instance part="R10" gate="G$1" x="287.02" y="25.4" rot="MR0"/>
            <instance part="S16" gate="1" x="287.02" y="7.62" rot="MR0"/>
            <instance part="J6" gate="+" x="284.48" y="17.78"/>
            <instance part="J6" gate="-" x="284.48" y="12.7"/>
            <instance part="R5" gate="G$1" x="220.98" y="78.74"/>
            <instance part="S17" gate="1" x="228.6" y="73.66"/>
            <instance part="S18" gate="1" x="101.6" y="40.64"/>
            <instance part="S19" gate="1" x="91.44" y="60.96"/>
            <instance part="S20" gate="1" x="119.38" y="40.64"/>
            <instance part="IC1" gate="A" x="129.54" y="48.26" rot="MR180"/>
            <instance part="IC1" gate="B" x="203.2" y="40.64" rot="MR180"/>
            <instance part="IC5" gate="A" x="276.86" y="33.02" rot="MR180"/>
            <instance part="IC5" gate="B" x="20.32" y="81.28" rot="MR180"/>
            <instance part="IC5" gate="P" x="20.32" y="81.28" rot="MR0"/>
          </instances>
          <busses>
</busses>
          <nets>
            <net name="GND" class="0">
              <segment>
                <pinref part="J2" gate="-" pin="P$1"/>
                <wire x1="-33.02" y1="63.5" x2="-27.94" y2="63.5" width="0.1524" layer="91"/>
                <pinref part="S2" gate="1" pin="GND"/>
                <wire x1="-27.94" y1="63.5" x2="-27.94" y2="60.96" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="J1" gate="-" pin="P$1"/>
                <wire x1="-33.02" y1="78.74" x2="-27.94" y2="78.74" width="0.1524" layer="91"/>
                <pinref part="S1" gate="1" pin="GND"/>
                <wire x1="-27.94" y1="78.74" x2="-27.94" y2="76.2" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R29" gate="1" pin="A"/>
                <pinref part="S3" gate="1" pin="GND"/>
                <wire x1="-20.32" y1="53.34" x2="-20.32" y2="55.88" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="S4" gate="1" pin="GND"/>
                <wire x1="7.62" y1="73.66" x2="7.62" y2="78.74" width="0.1524" layer="91"/>
                <wire x1="7.62" y1="78.74" x2="12.7" y2="78.74" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="B" pin="+IN"/>
              </segment>
              <segment>
                <pinref part="R7" gate="G$1" pin="2"/>
                <pinref part="S5" gate="1" pin="GND"/>
                <wire x1="33.02" y1="55.88" x2="33.02" y2="53.34" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="T1" gate="G$1" pin="B"/>
                <wire x1="38.1" y1="40.64" x2="38.1" y2="43.18" width="0.1524" layer="91"/>
                <wire x1="38.1" y1="43.18" x2="22.86" y2="43.18" width="0.1524" layer="91"/>
                <pinref part="S6" gate="1" pin="GND"/>
                <wire x1="22.86" y1="43.18" x2="22.86" y2="40.64" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="IC4" gate="A" pin="+IN"/>
                <pinref part="S7" gate="1" pin="GND"/>
                <wire x1="27.94" y1="-5.08" x2="27.94" y2="-2.54" width="0.1524" layer="91"/>
                <wire x1="27.94" y1="-2.54" x2="30.48" y2="-2.54" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R1" gate="G$1" pin="2"/>
                <pinref part="S8" gate="1" pin="GND"/>
                <wire x1="154.94" y1="33.02" x2="154.94" y2="35.56" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="J4" gate="-" pin="P$1"/>
                <pinref part="S9" gate="1" pin="GND"/>
                <wire x1="137.16" y1="27.94" x2="139.7" y2="27.94" width="0.1524" layer="91"/>
                <wire x1="139.7" y1="27.94" x2="139.7" y2="25.4" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="S10" gate="1" pin="GND"/>
                <wire x1="193.04" y1="35.56" x2="193.04" y2="38.1" width="0.1524" layer="91"/>
                <wire x1="193.04" y1="38.1" x2="195.58" y2="38.1" width="0.1524" layer="91"/>
                <pinref part="IC1" gate="B" pin="+IN"/>
              </segment>
              <segment>
                <pinref part="J5" gate="-" pin="P$1"/>
                <pinref part="S11" gate="1" pin="GND"/>
                <wire x1="210.82" y1="20.32" x2="213.36" y2="20.32" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="20.32" x2="213.36" y2="17.78" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R3" gate="G$1" pin="2"/>
                <pinref part="S12" gate="1" pin="GND"/>
                <wire x1="228.6" y1="25.4" x2="228.6" y2="27.94" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="S13" gate="1" pin="GND"/>
                <wire x1="266.7" y1="27.94" x2="266.7" y2="30.48" width="0.1524" layer="91"/>
                <wire x1="266.7" y1="30.48" x2="269.24" y2="30.48" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="A" pin="+IN"/>
              </segment>
              <segment>
                <pinref part="R2" gate="G$1" pin="2"/>
                <pinref part="S14" gate="1" pin="GND"/>
                <wire x1="162.56" y1="-7.62" x2="162.56" y2="-5.08" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R4" gate="G$1" pin="2"/>
                <pinref part="S15" gate="1" pin="GND"/>
                <wire x1="236.22" y1="-15.24" x2="236.22" y2="-12.7" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="J6" gate="-" pin="P$1"/>
                <pinref part="S16" gate="1" pin="GND"/>
                <wire x1="284.48" y1="12.7" x2="287.02" y2="12.7" width="0.1524" layer="91"/>
                <wire x1="287.02" y1="12.7" x2="287.02" y2="10.16" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R5" gate="G$1" pin="2"/>
                <pinref part="S17" gate="1" pin="GND"/>
                <wire x1="226.06" y1="78.74" x2="228.6" y2="78.74" width="0.1524" layer="91"/>
                <wire x1="228.6" y1="78.74" x2="228.6" y2="76.2" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R32" gate="1" pin="E"/>
                <pinref part="S18" gate="1" pin="GND"/>
                <wire x1="101.6" y1="45.72" x2="101.6" y2="43.18" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="J3" gate="-" pin="P$1"/>
                <pinref part="S19" gate="1" pin="GND"/>
                <wire x1="88.9" y1="66.04" x2="91.44" y2="66.04" width="0.1524" layer="91"/>
                <wire x1="91.44" y1="66.04" x2="91.44" y2="63.5" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="S20" gate="1" pin="GND"/>
                <wire x1="121.92" y1="45.72" x2="119.38" y2="45.72" width="0.1524" layer="91"/>
                <wire x1="119.38" y1="45.72" x2="119.38" y2="43.18" width="0.1524" layer="91"/>
                <pinref part="IC1" gate="A" pin="+IN"/>
              </segment>
            </net>
            <net name="VCC" class="0">
              <segment>
                <pinref part="U$1" gate="G$1" pin="VCC"/>
                <pinref part="R30" gate="1" pin="A"/>
                <wire x1="-35.56" y1="45.72" x2="-35.56" y2="43.18" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="U$2" gate="G$1" pin="VCC"/>
                <pinref part="R31" gate="1" pin="A"/>
                <wire x1="-35.56" y1="17.78" x2="-35.56" y2="15.24" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="U$7" gate="G$1" pin="VCC"/>
                <wire x1="20.32" y1="88.9" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="P" pin="V+"/>
              </segment>
              <segment>
                <pinref part="IC4" gate="A" pin="OPT"/>
                <pinref part="U$9" gate="G$1" pin="VCC"/>
                <wire x1="38.1" y1="7.62" x2="38.1" y2="10.16" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R27" gate="1" pin="A"/>
                <pinref part="U$11" gate="G$1" pin="VCC"/>
                <wire x1="144.78" y1="15.24" x2="144.78" y2="12.7" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R28" gate="1" pin="A"/>
                <pinref part="U$13" gate="G$1" pin="VCC"/>
                <wire x1="218.44" y1="7.62" x2="218.44" y2="5.08" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="VEE" class="0">
              <segment>
                <pinref part="R30" gate="1" pin="E"/>
                <pinref part="U$3" gate="G$1" pin="VEE"/>
                <wire x1="-35.56" y1="30.48" x2="-35.56" y2="33.02" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R31" gate="1" pin="E"/>
                <pinref part="U$4" gate="G$1" pin="VEE"/>
                <wire x1="-35.56" y1="2.54" x2="-35.56" y2="5.08" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="U$5" gate="G$1" pin="VEE"/>
                <pinref part="R21" gate="G$1" pin="1"/>
                <wire x1="10.16" y1="2.54" x2="15.24" y2="2.54" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="U$6" gate="G$1" pin="VEE"/>
                <wire x1="20.32" y1="73.66" x2="20.32" y2="76.2" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="P" pin="V-"/>
              </segment>
              <segment>
                <pinref part="U$8" gate="G$1" pin="VEE"/>
                <wire x1="38.1" y1="-10.16" x2="38.1" y2="-7.62" width="0.1524" layer="91"/>
                <pinref part="IC4" gate="A" pin="ON1"/>
              </segment>
              <segment>
                <pinref part="R27" gate="1" pin="E"/>
                <pinref part="U$10" gate="G$1" pin="VEE"/>
                <wire x1="144.78" y1="0" x2="144.78" y2="2.54" width="0.1524" layer="91"/>
              </segment>
              <segment>
                <pinref part="R28" gate="1" pin="E"/>
                <pinref part="U$12" gate="G$1" pin="VEE"/>
                <wire x1="218.44" y1="-7.62" x2="218.44" y2="-5.08" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$1" class="0">
              <segment>
                <pinref part="J1" gate="+" pin="P$1"/>
                <pinref part="R19" gate="G$1" pin="1"/>
                <wire x1="-33.02" y1="83.82" x2="-12.7" y2="83.82" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$2" class="0">
              <segment>
                <pinref part="R18" gate="G$1" pin="1"/>
                <pinref part="R29" gate="1" pin="S"/>
                <wire x1="-12.7" y1="60.96" x2="-17.78" y2="60.96" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$3" class="0">
              <segment>
                <pinref part="R29" gate="1" pin="E"/>
                <pinref part="J2" gate="+" pin="P$1"/>
                <wire x1="-20.32" y1="66.04" x2="-20.32" y2="68.58" width="0.1524" layer="91"/>
                <wire x1="-20.32" y1="68.58" x2="-33.02" y2="68.58" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$4" class="0">
              <segment>
                <pinref part="R30" gate="1" pin="S"/>
                <pinref part="R20" gate="G$1" pin="1"/>
                <wire x1="-33.02" y1="38.1" x2="-12.7" y2="38.1" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$5" class="0">
              <segment>
                <pinref part="R31" gate="1" pin="S"/>
                <pinref part="R34" gate="G$1" pin="1"/>
                <wire x1="-33.02" y1="10.16" x2="-12.7" y2="10.16" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$6" class="0">
              <segment>
                <pinref part="R34" gate="G$1" pin="2"/>
                <wire x1="-2.54" y1="10.16" x2="0" y2="10.16" width="0.1524" layer="91"/>
                <pinref part="R20" gate="G$1" pin="2"/>
                <wire x1="0" y1="10.16" x2="0" y2="38.1" width="0.1524" layer="91"/>
                <wire x1="0" y1="38.1" x2="-2.54" y2="38.1" width="0.1524" layer="91"/>
                <pinref part="R18" gate="G$1" pin="2"/>
                <wire x1="-2.54" y1="60.96" x2="0" y2="60.96" width="0.1524" layer="91"/>
                <wire x1="0" y1="60.96" x2="0" y2="38.1" width="0.1524" layer="91"/>
                <junction x="0" y="38.1"/>
                <pinref part="R19" gate="G$1" pin="2"/>
                <wire x1="-2.54" y1="83.82" x2="0" y2="83.82" width="0.1524" layer="91"/>
                <wire x1="0" y1="83.82" x2="0" y2="60.96" width="0.1524" layer="91"/>
                <junction x="0" y="60.96"/>
                <wire x1="0" y1="83.82" x2="12.7" y2="83.82" width="0.1524" layer="91"/>
                <junction x="0" y="83.82"/>
                <pinref part="IC5" gate="B" pin="-IN"/>
              </segment>
            </net>
            <net name="N$7" class="0">
              <segment>
                <wire x1="27.94" y1="81.28" x2="33.02" y2="81.28" width="0.1524" layer="91"/>
                <pinref part="R6" gate="1" pin="A"/>
                <wire x1="33.02" y1="81.28" x2="33.02" y2="78.74" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="B" pin="OUT"/>
              </segment>
            </net>
            <net name="N$8" class="0">
              <segment>
                <pinref part="R6" gate="1" pin="E"/>
                <pinref part="R7" gate="G$1" pin="1"/>
                <wire x1="33.02" y1="68.58" x2="33.02" y2="66.04" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$9" class="0">
              <segment>
                <pinref part="T1" gate="G$1" pin="C"/>
                <pinref part="T2" gate="G$1" pin="E"/>
                <wire x1="43.18" y1="35.56" x2="48.26" y2="35.56" width="0.1524" layer="91"/>
                <pinref part="R12" gate="G$1" pin="1"/>
                <wire x1="48.26" y1="35.56" x2="53.34" y2="35.56" width="0.1524" layer="91"/>
                <wire x1="48.26" y1="35.56" x2="48.26" y2="30.48" width="0.1524" layer="91"/>
                <junction x="48.26" y="35.56"/>
              </segment>
            </net>
            <net name="N$10" class="0">
              <segment>
                <pinref part="R12" gate="G$1" pin="2"/>
                <pinref part="C5" gate="G$1" pin="2"/>
                <wire x1="48.26" y1="20.32" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
                <wire x1="48.26" y1="17.78" x2="40.64" y2="17.78" width="0.1524" layer="91"/>
                <pinref part="IC4" gate="A" pin="OUT"/>
                <wire x1="45.72" y1="0" x2="48.26" y2="0" width="0.1524" layer="91"/>
                <wire x1="48.26" y1="0" x2="48.26" y2="17.78" width="0.1524" layer="91"/>
                <junction x="48.26" y="17.78"/>
              </segment>
            </net>
            <net name="N$11" class="0">
              <segment>
                <pinref part="C5" gate="G$1" pin="1"/>
                <wire x1="35.56" y1="17.78" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
                <pinref part="T1" gate="G$1" pin="E"/>
                <wire x1="27.94" y1="17.78" x2="27.94" y2="35.56" width="0.1524" layer="91"/>
                <wire x1="27.94" y1="35.56" x2="33.02" y2="35.56" width="0.1524" layer="91"/>
                <pinref part="IC4" gate="A" pin="-IN"/>
                <wire x1="30.48" y1="2.54" x2="27.94" y2="2.54" width="0.1524" layer="91"/>
                <wire x1="27.94" y1="2.54" x2="27.94" y2="17.78" width="0.1524" layer="91"/>
                <junction x="27.94" y="17.78"/>
                <pinref part="R21" gate="G$1" pin="2"/>
                <wire x1="25.4" y1="2.54" x2="27.94" y2="2.54" width="0.1524" layer="91"/>
                <junction x="27.94" y="2.54"/>
              </segment>
            </net>
            <net name="N$12" class="0">
              <segment>
                <pinref part="R6" gate="1" pin="S"/>
                <wire x1="35.56" y1="73.66" x2="58.42" y2="73.66" width="0.1524" layer="91"/>
                <pinref part="T2" gate="G$1" pin="B"/>
                <wire x1="58.42" y1="73.66" x2="58.42" y2="40.64" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="IABC" class="0">
              <segment>
                <pinref part="T2" gate="G$1" pin="C"/>
                <wire x1="63.5" y1="35.56" x2="76.2" y2="35.56" width="0.1524" layer="91"/>
                <label x="71.12" y="35.56" size="1.778" layer="95" font="fixed"/>
              </segment>
              <segment>
                <pinref part="R15" gate="G$1" pin="1"/>
                <wire x1="185.42" y1="60.96" x2="185.42" y2="68.58" width="0.1524" layer="91"/>
                <label x="185.42" y="63.5" size="1.778" layer="95" font="fixed" rot="R90"/>
              </segment>
              <segment>
                <pinref part="R16" gate="G$1" pin="1"/>
                <wire x1="259.08" y1="53.34" x2="259.08" y2="60.96" width="0.1524" layer="91"/>
                <label x="259.08" y="55.88" size="1.778" layer="95" font="fixed" rot="R90"/>
              </segment>
            </net>
            <net name="N$13" class="0">
              <segment>
                <pinref part="R32" gate="1" pin="S"/>
                <pinref part="R22" gate="G$1" pin="1"/>
                <wire x1="106.68" y1="50.8" x2="104.14" y2="50.8" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$14" class="0">
              <segment>
                <pinref part="R22" gate="G$1" pin="2"/>
                <wire x1="119.38" y1="50.8" x2="116.84" y2="50.8" width="0.1524" layer="91"/>
                <wire x1="119.38" y1="50.8" x2="119.38" y2="58.42" width="0.1524" layer="91"/>
                <pinref part="R24" gate="G$1" pin="1"/>
                <wire x1="119.38" y1="58.42" x2="124.46" y2="58.42" width="0.1524" layer="91"/>
                <pinref part="R17" gate="G$1" pin="1"/>
                <wire x1="124.46" y1="71.12" x2="119.38" y2="71.12" width="0.1524" layer="91"/>
                <wire x1="119.38" y1="71.12" x2="119.38" y2="58.42" width="0.1524" layer="91"/>
                <junction x="119.38" y="58.42"/>
                <pinref part="R23" gate="G$1" pin="1"/>
                <wire x1="124.46" y1="83.82" x2="119.38" y2="83.82" width="0.1524" layer="91"/>
                <wire x1="119.38" y1="83.82" x2="119.38" y2="71.12" width="0.1524" layer="91"/>
                <junction x="119.38" y="71.12"/>
                <wire x1="121.92" y1="50.8" x2="119.38" y2="50.8" width="0.1524" layer="91"/>
                <junction x="119.38" y="50.8"/>
                <pinref part="IC1" gate="A" pin="-IN"/>
              </segment>
            </net>
            <net name="N$15" class="0">
              <segment>
                <pinref part="R13" gate="G$1" pin="1"/>
                <wire x1="142.24" y1="48.26" x2="139.7" y2="48.26" width="0.1524" layer="91"/>
                <pinref part="R24" gate="G$1" pin="2"/>
                <wire x1="139.7" y1="48.26" x2="137.16" y2="48.26" width="0.1524" layer="91"/>
                <wire x1="134.62" y1="58.42" x2="139.7" y2="58.42" width="0.1524" layer="91"/>
                <wire x1="139.7" y1="58.42" x2="139.7" y2="48.26" width="0.1524" layer="91"/>
                <junction x="139.7" y="48.26"/>
                <pinref part="C1" gate="G$1" pin="1"/>
                <wire x1="139.7" y1="58.42" x2="144.78" y2="58.42" width="0.1524" layer="91"/>
                <junction x="139.7" y="58.42"/>
                <pinref part="R8" gate="G$1" pin="1"/>
                <wire x1="139.7" y1="45.72" x2="139.7" y2="48.26" width="0.1524" layer="91"/>
                <pinref part="IC1" gate="A" pin="OUT"/>
              </segment>
            </net>
            <net name="N$16" class="0">
              <segment>
                <pinref part="C1" gate="G$1" pin="2"/>
                <wire x1="149.86" y1="58.42" x2="154.94" y2="58.42" width="0.1524" layer="91"/>
                <wire x1="154.94" y1="58.42" x2="154.94" y2="48.26" width="0.1524" layer="91"/>
                <pinref part="R13" gate="G$1" pin="2"/>
                <wire x1="154.94" y1="48.26" x2="152.4" y2="48.26" width="0.1524" layer="91"/>
                <pinref part="IC2" gate="A" pin="-IN"/>
                <wire x1="165.1" y1="48.26" x2="154.94" y2="48.26" width="0.1524" layer="91"/>
                <junction x="154.94" y="48.26"/>
                <pinref part="R1" gate="G$1" pin="1"/>
                <wire x1="154.94" y1="45.72" x2="154.94" y2="48.26" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$17" class="0">
              <segment>
                <pinref part="J4" gate="+" pin="P$1"/>
                <pinref part="R8" gate="G$1" pin="2"/>
                <wire x1="137.16" y1="33.02" x2="139.7" y2="33.02" width="0.1524" layer="91"/>
                <wire x1="139.7" y1="33.02" x2="139.7" y2="35.56" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$18" class="0">
              <segment>
                <pinref part="R15" gate="G$1" pin="2"/>
                <pinref part="IC2" gate="A" pin="AMP/BIAS"/>
                <wire x1="185.42" y1="50.8" x2="185.42" y2="48.26" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$19" class="0">
              <segment>
                <pinref part="IC2" gate="A" pin="OUT"/>
                <wire x1="195.58" y1="43.18" x2="193.04" y2="43.18" width="0.1524" layer="91"/>
                <wire x1="193.04" y1="43.18" x2="190.5" y2="43.18" width="0.1524" layer="91"/>
                <wire x1="193.04" y1="43.18" x2="193.04" y2="53.34" width="0.1524" layer="91"/>
                <junction x="193.04" y="43.18"/>
                <pinref part="C3" gate="G$1" pin="1"/>
                <wire x1="193.04" y1="53.34" x2="200.66" y2="53.34" width="0.1524" layer="91"/>
                <pinref part="IC1" gate="B" pin="-IN"/>
              </segment>
            </net>
            <net name="N$20" class="0">
              <segment>
                <pinref part="C3" gate="G$1" pin="2"/>
                <wire x1="205.74" y1="53.34" x2="213.36" y2="53.34" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="53.34" x2="213.36" y2="40.64" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="40.64" x2="210.82" y2="40.64" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="53.34" x2="218.44" y2="53.34" width="0.1524" layer="91"/>
                <junction x="213.36" y="53.34"/>
                <pinref part="C2" gate="G$1" pin="1"/>
                <pinref part="R14" gate="G$1" pin="1"/>
                <wire x1="215.9" y1="40.64" x2="213.36" y2="40.64" width="0.1524" layer="91"/>
                <junction x="213.36" y="40.64"/>
                <pinref part="R9" gate="G$1" pin="1"/>
                <wire x1="213.36" y1="38.1" x2="213.36" y2="40.64" width="0.1524" layer="91"/>
                <pinref part="R33" gate="1" pin="A"/>
                <wire x1="213.36" y1="66.04" x2="213.36" y2="53.34" width="0.1524" layer="91"/>
                <pinref part="IC1" gate="B" pin="OUT"/>
              </segment>
            </net>
            <net name="N$21" class="0">
              <segment>
                <pinref part="J5" gate="+" pin="P$1"/>
                <pinref part="R9" gate="G$1" pin="2"/>
                <wire x1="210.82" y1="25.4" x2="213.36" y2="25.4" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="25.4" x2="213.36" y2="27.94" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$22" class="0">
              <segment>
                <pinref part="C2" gate="G$1" pin="2"/>
                <wire x1="223.52" y1="53.34" x2="228.6" y2="53.34" width="0.1524" layer="91"/>
                <wire x1="228.6" y1="53.34" x2="228.6" y2="40.64" width="0.1524" layer="91"/>
                <pinref part="R14" gate="G$1" pin="2"/>
                <wire x1="228.6" y1="40.64" x2="226.06" y2="40.64" width="0.1524" layer="91"/>
                <pinref part="R3" gate="G$1" pin="1"/>
                <wire x1="228.6" y1="38.1" x2="228.6" y2="40.64" width="0.1524" layer="91"/>
                <junction x="228.6" y="40.64"/>
                <pinref part="IC2" gate="B" pin="-IN"/>
                <wire x1="238.76" y1="40.64" x2="228.6" y2="40.64" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$23" class="0">
              <segment>
                <pinref part="IC2" gate="B" pin="+IN"/>
                <wire x1="238.76" y1="30.48" x2="236.22" y2="30.48" width="0.1524" layer="91"/>
                <wire x1="236.22" y1="30.48" x2="236.22" y2="10.16" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$24" class="0">
              <segment>
                <pinref part="R27" gate="1" pin="S"/>
                <pinref part="R25" gate="G$1" pin="1"/>
                <wire x1="147.32" y1="7.62" x2="149.86" y2="7.62" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$25" class="0">
              <segment>
                <pinref part="R25" gate="G$1" pin="2"/>
                <pinref part="R2" gate="G$1" pin="1"/>
                <wire x1="160.02" y1="7.62" x2="162.56" y2="7.62" width="0.1524" layer="91"/>
                <wire x1="162.56" y1="7.62" x2="162.56" y2="5.08" width="0.1524" layer="91"/>
                <wire x1="162.56" y1="7.62" x2="162.56" y2="38.1" width="0.1524" layer="91"/>
                <junction x="162.56" y="7.62"/>
                <pinref part="IC2" gate="A" pin="+IN"/>
                <wire x1="162.56" y1="38.1" x2="165.1" y2="38.1" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$26" class="0">
              <segment>
                <pinref part="R28" gate="1" pin="S"/>
                <pinref part="R26" gate="G$1" pin="1"/>
                <wire x1="220.98" y1="0" x2="223.52" y2="0" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$27" class="0">
              <segment>
                <pinref part="R26" gate="G$1" pin="2"/>
                <pinref part="R4" gate="G$1" pin="1"/>
                <wire x1="233.68" y1="0" x2="236.22" y2="0" width="0.1524" layer="91"/>
                <wire x1="236.22" y1="0" x2="236.22" y2="-2.54" width="0.1524" layer="91"/>
                <wire x1="236.22" y1="0" x2="236.22" y2="30.48" width="0.1524" layer="91"/>
                <junction x="236.22" y="0"/>
              </segment>
            </net>
            <net name="N$28" class="0">
              <segment>
                <pinref part="R16" gate="G$1" pin="2"/>
                <wire x1="259.08" y1="43.18" x2="259.08" y2="40.64" width="0.1524" layer="91"/>
                <pinref part="IC2" gate="B" pin="AMP/BIAS"/>
              </segment>
            </net>
            <net name="N$29" class="0">
              <segment>
                <pinref part="IC2" gate="B" pin="OUT"/>
                <wire x1="264.16" y1="35.56" x2="266.7" y2="35.56" width="0.1524" layer="91"/>
                <wire x1="266.7" y1="35.56" x2="269.24" y2="35.56" width="0.1524" layer="91"/>
                <wire x1="266.7" y1="35.56" x2="266.7" y2="45.72" width="0.1524" layer="91"/>
                <junction x="266.7" y="35.56"/>
                <pinref part="C4" gate="G$1" pin="1"/>
                <wire x1="266.7" y1="45.72" x2="274.32" y2="45.72" width="0.1524" layer="91"/>
                <pinref part="IC5" gate="A" pin="-IN"/>
              </segment>
            </net>
            <net name="N$30" class="0">
              <segment>
                <pinref part="C4" gate="G$1" pin="2"/>
                <wire x1="279.4" y1="45.72" x2="287.02" y2="45.72" width="0.1524" layer="91"/>
                <wire x1="287.02" y1="45.72" x2="287.02" y2="33.02" width="0.1524" layer="91"/>
                <wire x1="287.02" y1="33.02" x2="284.48" y2="33.02" width="0.1524" layer="91"/>
                <pinref part="R10" gate="G$1" pin="1"/>
                <wire x1="287.02" y1="33.02" x2="287.02" y2="30.48" width="0.1524" layer="91"/>
                <junction x="287.02" y="33.02"/>
                <pinref part="R23" gate="G$1" pin="2"/>
                <wire x1="134.62" y1="83.82" x2="287.02" y2="83.82" width="0.1524" layer="91"/>
                <wire x1="287.02" y1="83.82" x2="287.02" y2="45.72" width="0.1524" layer="91"/>
                <junction x="287.02" y="45.72"/>
                <pinref part="IC5" gate="A" pin="OUT"/>
              </segment>
            </net>
            <net name="N$31" class="0">
              <segment>
                <pinref part="J6" gate="+" pin="P$1"/>
                <pinref part="R10" gate="G$1" pin="2"/>
                <wire x1="284.48" y1="17.78" x2="287.02" y2="17.78" width="0.1524" layer="91"/>
                <wire x1="287.02" y1="17.78" x2="287.02" y2="20.32" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$32" class="0">
              <segment>
                <pinref part="R17" gate="G$1" pin="2"/>
                <pinref part="R33" gate="1" pin="S"/>
                <wire x1="134.62" y1="71.12" x2="210.82" y2="71.12" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$33" class="0">
              <segment>
                <pinref part="R5" gate="G$1" pin="1"/>
                <pinref part="R33" gate="1" pin="E"/>
                <wire x1="215.9" y1="78.74" x2="213.36" y2="78.74" width="0.1524" layer="91"/>
                <wire x1="213.36" y1="78.74" x2="213.36" y2="76.2" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$34" class="0">
              <segment>
                <pinref part="J3" gate="+" pin="P$1"/>
                <pinref part="C14" gate="1" pin="1"/>
                <wire x1="88.9" y1="71.12" x2="101.6" y2="71.12" width="0.1524" layer="91"/>
                <wire x1="101.6" y1="71.12" x2="101.6" y2="63.5" width="0.1524" layer="91"/>
              </segment>
            </net>
            <net name="N$35" class="0">
              <segment>
                <pinref part="C14" gate="1" pin="2"/>
                <pinref part="R32" gate="1" pin="A"/>
                <wire x1="101.6" y1="58.42" x2="101.6" y2="55.88" width="0.1524" layer="91"/>
              </segment>
            </net>
          </nets>
        </sheet>
        <sheet>
          <plain>
</plain>
          <instances>
</instances>
          <busses>
</busses>
          <nets>
</nets>
        </sheet>
      </sheets>
    </schematic>
  </drawing>
</eagle>
