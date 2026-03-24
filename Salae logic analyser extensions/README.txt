These are extensions I made for the Salae Analyser 2 Logic 2 app.

FSK analyser is used to decode 2FSK data, but outputs the bit stream
Byte Decoder is used to decode 2FSK data, but outputs the byte stream

To use them go into the extensions tab in the Logic 2 app (can be found at the side maybe), click the triple dots -> load existing extensions -> Load the extensions json from the respective files.

Then go into analysers
1)Add a simple parallel analyser -> select the channel to analyse -> Set clock to the same channel -> Set clock state to dual edge
2)Add either FSK Analyser or Byte decoder analyser -> Set input analyser as simple parallel, a good tolerance percent is anywhere between 0 to 5 -> set bitrate as the FSK bitrate -> set the centre frequence as (f0 + f1)/2.