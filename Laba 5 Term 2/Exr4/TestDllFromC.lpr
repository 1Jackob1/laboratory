program TestDllFromC;

uses ULibLink;
var
  test: Pointer = nil;
  s: PChar = 'hello';
  err: Integer;
begin
  writeln('its okay');
  err:=STRING_new(@test, s);
  write('STRING_new(@test, s) code: ', err, #10);
  STRING_print(test);
  writeln('---') ;
  err:=
  readln();
  readln();
end.

