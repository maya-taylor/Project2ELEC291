# import speech_recognition as sr


# def send_voice_command():
#     # use the audio file as the audio source
#     recognizer = sr.Recognizer()

#     # Capture audio from the microphone
#     with sr.Microphone() as source:
#         print("Listening...")
        
#         audio = recognizer.listen(source, timeout=3,phrase_time_limit=3)

#     # Recognize speech using Google Speech Recognition
#     try:
#         print("Recognizing...")
#         text = recognizer.recognize_google(audio).lower() #lower case audio
#         print(f"You said: {text}")
#     except sr.UnknownValueError:
#         print("Could not understand audio")
#     except sr.RequestError as e:
#         print(f"Could not request results; {e}")

#     # do NLP on `text`

#     # matching against "drive straight" 
#     if "straight" in text:
#         voice_char = '&'
#     if "forward" in text:
#         voice_char = '&'

#     if "backward" in text:
#         voice_char = '{'
#     if "back" in text:
#         voice_char = '{'

#     if "left" in text:
#         voice_char = '|'
#     if "right" in text:
#         voice_char = '-'

#     if "square" in text:
#         voice_char = '/'
#     if "circle" in text:
#         voice_char = "'"

#     if "figure" in text:
#         voice_char = '"'
#     if "eight" in text:
#         voice_char = '"'

#     ser.write(f"{voice_char}\r\n".encode())
