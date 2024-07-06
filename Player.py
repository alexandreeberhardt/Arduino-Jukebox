import spotipy
from spotipy.oauth2 import SpotifyOAuth
import serial
from dotenv import load_dotenv
import os

load_dotenv()

sp = spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=os.getenv('SPOTIPY_CLIENT_ID'),
                                               client_secret=os.getenv('SPOTIPY_CLIENT_SECRET'),
                                               redirect_uri=os.getenv('SPOTIPY_REDIRECT_URI'),
                                               scope='user-modify-playback-state user-read-playback-state'))

ser = serial.Serial('/dev/ttyACM0', 9600)

def read_serial_input():
    if ser.in_waiting > 0:
        track_id = ser.readline().decode('utf-8').strip()
        print(track_id)
        return track_id
    return None

while True:
    track_id = read_serial_input()
    if track_id:
        print(f"Ajout du son à la file d'attente")
        sp.add_to_queue(track_id)
        sp.next_track() 
