import chess

class MoveData():
    def __init__(self, move: chess.Move, board: chess.Board, eval: dict) -> None:
        self.move = move
        self.board = board
        self.eval = eval
        self.player = board.turn
        self._count_material()
    
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

        

