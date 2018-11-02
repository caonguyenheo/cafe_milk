/* 
 *  Copyright (c) 2008-2015 Texas Instruments Incorporated
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 *
 *  Contributors:
 *      Texas Instruments - initial implementation
 *
 * */

/*
 *  ======== C674_big_endian.xdc ========
 *
 */

/*!
 *  ======== C674_big_endian ========
 *  TI C674 big endian
 */
metaonly module C674_big_endian inherits ti.targets.ITarget {
    override readonly config string name                 = "C674_big_endian";
    override readonly config string suffix               = "674e";
    override readonly config string isa                  = "674";
    override readonly config xdc.bld.ITarget.Model model = {
        endian: "big"
    };
    override readonly config xdc.bld.ITarget.Module base = ti.targets.C64P;

    /*
     *  ======== compatibleSuffixes ========
     */
    override config String compatibleSuffixes[] = ["64Pe", "64e", "62e"];

    override readonly config string rts = "ti.targets.rts6000";
    override config string platform     = "ti.platforms.evmDA830";

    override readonly config xdc.bld.ITarget2.Command ar = {
        cmd: "ar6x",
        opts: "rq"
    };

    override readonly config xdc.bld.ITarget2.Command cc = {
        cmd: "cl6x -c",
        opts: "-me -mv6740"
    };

    override readonly config xdc.bld.ITarget2.Command vers = {
        cmd: "cl6x",
        opts: "--compiler_revision"
    };

    override readonly config xdc.bld.ITarget2.Command asm = {
        cmd: "cl6x -c",
        opts: "-me -mv6740"
    };

    override readonly config xdc.bld.ITarget2.Command lnk = {
        cmd: "cl6x",
        opts: "-z"
    };

    /*!
     *  ======== asmOpts ========
     *  User configurable assembler options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     */
    override config xdc.bld.ITarget2.Options asmOpts = {
        prefix: "-qq",
        suffix: ""
    };

    /*!
     *  ======== ccOpts ========
     *  User configurable compiler options.
     *
     *  Defaults:
     *  @p(dlist)
     *      -`-qq`
     *          super quiet mode
     *      -`-pdsw225`
     *          generate a warning for implicitly declared functions; i.e.,
     *          functions without prototypes
     */
    override config xdc.bld.ITarget2.Options ccOpts = {
        prefix: "-qq -pdsw225",
        suffix: ""
    };

    /*!
     *  ======== ccConfigOpts ========
     *  User configurable compiler options for the generated config C file.
     *
     *  -mo places all functions into subsections
     *  --no_compress helps with compile time with no real difference in
     *  code size since the generated config.c is mostly data and small
     *  function stubs.
     */
    override config xdc.bld.ITarget2.Options ccConfigOpts = {
        prefix: "$(ccOpts.prefix) -mo --no_compress",
        suffix: "$(ccOpts.suffix)"
    };

    override config string includeOpts = "-I$(rootDir)/include";

    final override readonly config string sectMap[string] =
        ti.targets.C64P.sectMap;

    final override readonly config Bool splitMap[string] =
        ti.targets.C64P.splitMap;

    override readonly config xdc.bld.ITarget.StdTypes stdTypes =
        ti.targets.C64P.stdTypes;

    override readonly config Int bitsPerChar =
        ti.targets.C64P.bitsPerChar;
}
/*
 *  @(#) ti.targets; 1, 0, 3,0; 7-20-2018 13:58:59; /db/ztree/library/trees/xdctargets/xdctargets-r09/src/ xlibrary

 */

