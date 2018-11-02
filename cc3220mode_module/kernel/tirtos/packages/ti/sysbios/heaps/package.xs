/*
 * Copyright (c) 2012-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== package.xs ========
 */ 

function init() 
{
    /* Add HeapMem's primaryHeap addresses range to ROV's memory sections */
    if (xdc.om.$name == "rov") {
        var HeapMem = xdc.module('ti.sysbios.heaps.HeapMem');
//        if (xdc.om['ti.sysbios.heaps'].HeapMem.$used) {
            var Program = xdc.useModule('xdc.rov.Program');
            var HeapMemCfg =  Program.getModuleConfig(HeapMem.$name);
            if  (HeapMemCfg.primaryHeapBaseAddr != null) {
                var base = Program.getSymbolValue(HeapMemCfg.primaryHeapBaseAddr.substr(1));
                var end = Program.getSymbolValue(HeapMemCfg.primaryHeapEndAddr.substr(1));
                /* Retrieve the MemoryImage java object. */
                var Model = xdc.module("xdc.rov.Model");
                var memReader = Model.getMemoryImageInst();

                /* retrieve the sections list and add a new section */
                var sections = memReader.getSections();
                sections.addSection(base, end-base);
            }
//        }
    }
}

/*
 *  ======== Package.getLibs ========
 *  This function is called when a program's configuration files are
 *  being generated and it returns the name of a library appropriate
 *  for the program's configuration.
 */

function getLibs(prog)
{
    var Build = xdc.module("ti.sysbios.Build");

    /* use shared getLibs() */
    return (Build.getLibs(this));
}

