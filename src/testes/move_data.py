import chess

class MoveData():
    def __init__(self, move: chess.Move, board: chess.Board, eval: dict, previous_eval: dict) -> None:
        self.move = move
        self.board = board
        self.eval = eval
        self.player = board.turn
        self._count_material()
        self._evaluate_quality(previous_eval)
    
    def piece_moved(self):
        return self.board.piece_at(self.to_square).symbol()
    
    def _count_material(self):
        piece_values = {
            chess.PAWN: 1,
            chess.KNIGHT: 3,
            chess.BISHOP: 3,
            chess.ROOK: 5,
            chess.QUEEN: 9
        }

        self.white_material = 0
        self.black_material = 0

        piece_map = self.board.piece_map()

        for piece in piece_map.values():
            value = piece_values.get(piece.piece_type, 0)
            if piece.color == chess.WHITE:
                self.white_material += value
            else:
                self.black_material += value

    # def _evaluate_quality(self, previous_eval: dict):

    #     # From centipawn to centipawn
    #     if previous_eval['type'] == 'cp' and self.eval['type'] == 'cp':

    #         # Assuming the player will never make the evaluation increase, we calculate how much it decreased in absolute value
    #         decrease = abs(self.eval['value'] - previous_eval['value'])
            
    #         if decrease == 0:
    #             self.quality = 'Best'
    #         elif decrease < 50:
    #             self.quality = 'Excellent'
    #         elif decrease < 100: 
    #             self.quality = 'Good'
    #         elif decrease < 200:
    #             self.quality = 'Inaccuracy'
    #         elif decrease < 300:
    #             self.quality = 'Mistake'
    #         else:
    #             self.quality = 'Blunder'
    #     # Mate sequence changed to centipawn, a mate was missed
    #     elif previous_eval['type'] == 'mate' and self.eval['type'] == 'cp':
    #         self.quality = 'Missed Mate'
        
    #     # Centipawn changed to mate
    #     # Either one player blundered a mate, or stockfish had a high centipawn eval but hadn't seen the mate sequence
    #     elif previous_eval['type'] == 'cp' and self.eval['type'] == 'mate':
    #         if (self.player == chess.WHITE and )
    #         self.quality = 'Blunder'

    #     else: # both evals are of type mate

