# The Royal Game of Ur
This is the game of Ur, one of the oldest known board games.
Each player has a set of pawns racing along a track; you win by moving all of your pawns to the end, and you may send the other player's pawns back to their start.

# Rules
1. You roll (four binary) dice, and move one of your pawns by this many spaces; you may also forfeit your turn (with '.').
2. Your pawns cannot go past the end; you must have the exact right roll to go to the end space.
3. Pawns may be sent back to their start space, if the other player's pawn jumps on them. 
4. You may not jump your own pawns.
5. Spaces with a star allow you to re-roll, and are safe space(s).
6. The player who gets all of their pawns on their end space first wins.

# Dependancies
- ncurses

# Playing
- `mk.sh` builds and runs the game.
- Message shows current character (pawn symbol) and roll.
- Select the pawn you want to move with the arrow keys and hit enter, or press '.' to skip your turn.

# TODO
- [x] Base game logic and drawing.
- [ ] Add title screen and player option menu (to get name, colors, and pawn symbol).
- [ ] Networking!
- [ ] Better dialogue messages (maybe a small text box on the bottom).
- [ ] Tutorial and game rules screen(s).
- [ ] Handle small / large terminal sizes better.
