import requests

users = {
    'mrbarros': False,
}

api_url = "https://api.chess.com/pub/player/"

# acessa os {jogador}/games/archives, pega todos os links

archives = "/games/archives"

# acessa cada um dos links e baixa o pgn de resposta de todos, acumulando ele em um único pgn por usuario ao terminar muda o valor no dicionario de users para True

# recupera todos os usernames unicos contra o quem o jogador jogou, salva eles no dicionario com valor False, para registrar que não foi pego os arquivos deste user

# procura algum usuario no dicionario para o qual não foi feito o processo e repete para este

### falta decidir o que fazer com o pgn de cada usuário; considerar adicionar o uuid de cada jogo para não salvar o dobro de dados