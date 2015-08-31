// This file exists solely as a hook for doxygen documentation

/*! \mainpage TyVis documentation top.
 
   \section intro Introduction 

   TyVis is a VHDL elaboration/simulation support library written in C++.
   It's intention is to bridge the gap between VHDL semantics, and the
   discrete event simulation library, warped, on which it depends.

   \section vhdlConstructs VHDL Constructs

   \subsection numeric Numeric Types

   See UniversalInteger, UniversalReal, and PhysicalLiteral.

   \subsection access Array Types

   See Array.

   \subsection files File Types

   See FileObject.

   \subsection records Record Types

   See Record.

   \subsection access Access Types

   See AccessObject.

   \subsection signals Signals

   See Signal.

   \subsection processes Processes

   Processes in VHDL are a key concept.  Conceptually all processes run
   concurrently with each other, even during sequential simulation.  When a
   TyVis simulation is run in parallel simulation mode, processes will run
   concurrently "for real" during simulation.

   The TyVis implementation of a process can be found in VHDLProcess.

   \section elaboration Elaboration

   \section simulation Simulation
*/
