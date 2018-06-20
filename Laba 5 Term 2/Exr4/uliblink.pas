unit ULibLink;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;
var
  LibSrc: String = 'libCstring';
  function STRING_new(hand: Pointer; buff: Pointer): Integer; cdecl; external 'libCstring';
  function STRING_off(hand: Pointer):Integer; cdecl; external 'libCstring';
  function STRING_set(hand: Pointer; src: Pointer): Integer; cdecl; external 'libCstring';
  function STRING_cpy(hand: Pointer; src: Pointer): Integer; cdecl; external 'libCstring';
  function STRING_sub(hand: Pointer; src: Pointer; start: DWord; size: DWord): Integer; cdecl; external 'libCstring';
  function STRING_cat(hand: Pointer; lhs: Pointer; rhs: Pointer): Integer; cdecl; external 'libCstring';
  function STRING_add(hand: Pointer; lhs: Pointer; tail: Char): Integer; cdecl; external 'libCstring';
  function STRING_cmp(lhs: Pointer; rhs: Pointer): Integer; cdecl; external 'libCstring';
  function STRING_get(hand: Pointer; ind: Integer): Integer; cdecl; external 'libCstring';
  function STRING_len(hand: Pointer): Integer; cdecl; external 'libCstring';
  procedure STRING_print(hand: Pointer); cdecl; external 'libCstring';
implementation

end.

