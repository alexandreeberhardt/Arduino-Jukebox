# Arduino-Jukebox

## An Arduino Jukebox Using RFID Cards to Switch Music

This is a short project that creates a jukebox using an Arduino (Mega 2560 R3) and an RFID-RC522 module. With this project, you can change your Spotify music by passing an RFID tag in front of the sensor.

### Project Steps

1. **Writing the Music ID**:
   - Use a file to write the music ID to the 4th to 6th blocks of your RFID tag.
2. **Reading the Music ID**:
   - Use a second file to read the music ID from the tag.
3. **Interacting with Spotify**:
   - Use a Python script to interact with the Spotify API to switch the music according to your tag.

### Requirements

- You will need a Spotify Client ID and Secret, which you can get at the [Spotify Developer Dashboard](https://developer.spotify.com/dashboard).

### Future Improvements

- Upgrade connectivity to use a Wi-Fi component.
- Use a Raspberry Pi to run the project locally.
- Add LED controls to change the lighting according to the music.
