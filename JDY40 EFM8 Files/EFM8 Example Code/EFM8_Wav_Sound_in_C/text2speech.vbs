' http://www.ericphelps.com/scripting/samples/Texttowave/TextToWave.txt
'
'This script creates a WAV audio file from plain text.
'
'Drop a plain text file on this script, and it will create an 
'identically-named WAV file (except for the file extension!).
'The default audio format is set in the script with the constant
'AUDIO_FORMAT, or you may supply a format as a number from 
'4 to 68. The audio format number is supplied as an OPTIONAL
'second argument to this script.
'
'Common formats: 8 (8KHz 8-bit mono), 16 (16KHz 8-bit mono), 
'35 (44KHz 16-bit Stereo), 65 (GSM 8KHz), 66 (GSM 11KHz)

Option Explicit
dim tts, fs, ts, ss, strFile, strText, blnError, lngFormat
Const ForReading = 1
Const SSFMCreateForWrite = 3
Const AUDIO_FORMAT = 65 'Please refer to the SAFT function for value description

	'Check argument count
	If WScript.Arguments.Count = 0 Then
		MsgBox "Drop a text file on this script and I'll convert it into a WAV sound file."
		WScript.Quit 1
	End If
	'Make sure the argument is a file
	Set fs = CreateObject("Scripting.FileSystemObject")
	strFile = WScript.Arguments(0)
	strFile = Trim(strFile)
	If strFile = "" Then
		MsgBox "I can't read nothing! Give me the name of a text file."
		WScript.Quit 1
	End If
	If InStr(strFile, "\\") <> 1 And InStr(strFile, ":\") <> 2 Then
		strFile = fs.GetAbsolutePathName(fs.BuildPath(fs.GetFolder(".").Path, strFile))
	End If
	If Not fs.FileExists(strFile) Then
		MsgBox "I can't locate a file named """ & WScript.Arguments(0) & """. Try using a fully-qualified path and file name."
		WScript.Quit 1
	End If
	'Is there an optional second argument for the audio format?
	If WScript.Arguments.Count > 1 Then
		lngFormat = WScript.Arguments(1)
		If Not IsNumeric(lngFormat) Then
			MsgBox "The second argument, if supplied, must be a NUMBER. Yours wasn't!"
			WScript.Quit 1
		End If
		lngFormat = Int(Abs(lngFormat)) - 1
	Else
		lngFormat = AUDIO_FORMAT - 1
	End If
	'Get the SAPI 5text-to-voice engine. 
	Set tts = Nothing
	Set tts = CreateObject("Sapi.SpVoice")
	If tts Is Nothing Then
		InputBox "You don't have SAPI 5! Please hop on the internet, download, and install the Microsoft SAPI 5 speech development kit from here:", "Error", "http://download.microsoft.com/download/speechSDK/SDK/5.1/WXP/EN-US/speechsdk51.exe"
		WScript.Quit 1
	End If
	'Read the file
	Status "Reading text file"
	Set ts = fs.OpenTextFile(WScript.Arguments(0), ForReading, True)
	strText = ts.ReadAll
	ts.Close
	Set ts = Nothing
	'Generate a file name for the output WAV file
	strFile = Left(strFile, InStrRev(strFile, ".")) & "wav"
	'Check to see that chosen audio format is supported
	Err.Clear
	blnError = True
	On Error Resume Next
	Do While blnError
		lngFormat = lngFormat + 1
		If fs.FileExists(strFile) Then fs.DeleteFile strFile
		Set ss = CreateObject("Sapi.SpFileStream")
		ss.Format.Type = lngFormat
		ss.Open strFile, SSFMCreateForWrite, False
		Set tts.AudioOutputStream = ss
		Err.Clear
		If Err.Number = 0 Then
			Status "USING:  Format " & lngFormat & " (" & SAFT(lngFormat) & ")"
			blnError = False 
		Else
			Status "FAILED: Format " & lngFormat & " (" & SAFT(lngFormat) & ")"
			Set tts.AudioOutputStream = Nothing
			ss.Close
			Set ss = Nothing
			blnError = True
		End If
	Loop
	'Convert text to WAV!
	Status "Converting to WAV"
	tts.Speak strText
	'Clean up
	ss.Close
	Set ss = Nothing
	Set tts = Nothing

Sub Status(strMessage)
	If Lcase(Right(Wscript.FullName, 12)) = "\cscript.exe" Then
		Wscript.Echo strMessage
	End If
End Sub

Function SAFT(lngSpeechAudioFormatType)
'Returns a text description of the format type
'Values from SpeechLib.SpeechAudioFormatType
Dim strSAFT, lngSAFT
	'Preserve input variable
	lngSAFT = lngSpeechAudioFormatType
	'Stop bad input
	If Not IsNumeric(lngSAFT) Then
		SAFT = "Unknown"
		Exit Function
	End If
	If lngSAFT < 4 Or lngSAFT > 68 Then
		SAFT = "Unknown"
		Exit Function
	End If
	'NORMAL for values between 0 and 39
	'stereo = add 1
	'16-bit = add 2
	'8KHz = 4
	'11KHz = 8
	'12KHz = 12
	'16KHz = 16
	'22KHz = 20
	'24KHz = 24
	'32KHz = 28
	'44KHz = 32
	'48KHz = 36
	If lngSAFT < 40 Then
		lngSAFT = lngSAFT - 4 'Offset so it starts at zero
		strSAFT = Split("8 11 12 16 22 24 32 44 48")(lngSAFT \ 4) & "KHz"
		strSAFT = strSAFT & " "
		strSAFT = strSAFT & Split("8 16")((lngSAFT And 2) \ 2) & "bit"
		strSAFT = strSAFT & " "
		strSAFT = strSAFT & Split("Mono Stereo")(lngSAFT And 1)
	End If
	'TrueSpeech_8kHz1BitMono = 40
	If lngSAFT = 40 Then
		strSAFT = "TrueSpeech 8KHz 1bit Mono"
	End If
	'stereo = add 1 to below values
	'CCITT_ALaw for values between 41 and 48
	'8KHz = 41
	'11KHz = 43
	'22KHz = 45
	'44KHz = 47
	'CCITT_uLaw for values between 49 and 56
	'8KHz = 49
	'11KHz = 51
	'22KHz = 53
	'44KHz = 55
	'ADPCM for values between 57 and 64
	'8KHz = 57
	'11KHz = 59
	'22KHz = 61
	'44KHz = 63
	If ((lngSAFT > 40) And (lngSAFT < 65)) Then
		lngSAFT = lngSAFT - 41 'Offset so it starts at zero
		strSAFT = Split("CCITT_ALaw CCITT_uLaw ADPCM")(lngSAFT \ 8)
		strSAFT = strSAFT & " "
		strSAFT = strSAFT & Split("8 11 22 44")((lngSAFT \ 2) And 3) & "KHz"
		strSAFT = strSAFT & " "
		strSAFT = strSAFT & Split("Mono Stereo")(lngSAFT And 1)
	End If
	'GSM_610 for values between 65 and 68 (no stereo)
	'8KHz = 65
	'11KHz = 66
	'22KHz = 67
	'44KHz = 68
	If lngSAFT > 64 Then
		Select Case lngSAFT
			Case 65
				strSAFT = "GSM_610 8KHz Mono"
			Case 66
				strSAFT = "GSM_610 11KHz Mono"
			Case 67
				strSAFT = "GSM_610 22KHz Mono"
			Case 68
				strSAFT = "GSM_610 44KHz Mono"
			Case Else
				strSAFT = "Unknown"
		End Select
	End If
	'Return the value
	SAFT = strSAFT
End Function
