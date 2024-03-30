Dim Arg
Set Arg = WScript.Arguments

If Arg.Length = 0 Then
   WScript.Sleep 2000
Else
   WScript.Sleep Arg(0)
End If
