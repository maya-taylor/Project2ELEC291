import pygame
import RPi.GPIO as GPIO

# Initialize Pygame
pygame.init()

# Initialize GPIO
BUTTON_PIN = 17  # Example GPIO pin (change to the pin connected to the button)
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)

# Load the Cha Cha Slide audio file
CHA_CHA_SLIDE_FILE = "cha_cha_slide.mp3"  # Change to the path of your audio file
pygame.mixer.music.load(CHA_CHA_SLIDE_FILE)

try:
    while True:
        # Wait for the button to be pressed
        GPIO.wait_for_edge(BUTTON_PIN, GPIO.FALLING)
        
        # Play the Cha Cha Slide audio
        pygame.mixer.music.play()
        
        # Wait for the audio to finish playing
        while pygame.mixer.music.get_busy():
            continue

finally:
    # Clean up GPIO
    GPIO.cleanup()
