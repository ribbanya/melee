/** @file
 * Definitions for the C that `melee-dat samples codegen` generates, written
 * next to it by `melee-dat samples macros`, like dtk's `macros.inc` for
 * its assembly. Not for the decomp's own code.
 */
#ifndef DAT_SAMPLES_MACROS_H
#define DAT_SAMPLES_MACROS_H

/// Archive data that the archive doesn't name: local to its unit, like the
/// target object's symbol, and kept where nothing in the unit points to it.
#define LOCAL static __attribute__((used))

/// A byte of archive data no chain of relocations from a public symbol
/// reaches, so the game can't use it. Written as arrays of its bytes, @c LOCAL
/// so that they're kept, e.g.
/// @c UNUSED LOCAL OrphanedData unused_x6A80[0x40] = { ... };
typedef unsigned char OrphanedData;

#endif
