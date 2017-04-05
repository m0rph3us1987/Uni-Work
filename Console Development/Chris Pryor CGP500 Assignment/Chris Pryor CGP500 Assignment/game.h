#pragma once

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//  These four functions will be called externally from main, and update our      //
//  game, initilise it.... render it...etc.                                       //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

bool Create();                            // Called once at start of program loading
void Release();                           // Called when program ends
void Update();                            // Just before Render is called
void Render();                            // Called in main render loop.