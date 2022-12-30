# Fake Wifi
<img width="100%" src="fakewifi/data/ig.png">

# Indice

- [Sobre](#-sobre)
- [Tecnologias](#Tecnologias)
- [Dispositivos](#Dispositivos)
- [Sketch](#Sketch)
- [Como baixar o projeto](#-como-baixar-o-projeto)

## 🔖&nbsp; Sobre

Projeto desenvolvido para obter dados de login do instagram.

É necessário configurar o ESP8266 upload file tool para enviar a imagem e o arquivo data.json para o SPIFFS para inicializar o projeto.

### Rotas

`/*` Method `GET` - Rota com a tela de login
`/creds` Method `GET` - Rota que lista os dados obtidos
`/clear` Method `GET` - Rota que apaga a lista da memória
`/post?username=<nome_de_usuario>&password=<password>&created_at=<timestamp>` Method `POST` - Rota que salva os dados na lista e apresenta a tela de espera


 ```
 # Adiciona a url de gerenciador de placas
 http://arduino.esp8266.com/stable/package_esp8266com_index.json
 ```

---
## Tecnologias

Esse projeto foi desenvolvido com as seguintes tecnologias:

- [C](https://isocpp.org/)
- [Arduino IDE](https://www.arduino.cc/)

## Dispositivos

- [NodeMCU]()

---

<h3 align="center" >Vamos nos conectar 😉</h3>
<p align="center">
  <a href="https://www.linkedin.com/in/filipelbatista/">
    <img alt="LinkedIn" width="22px" src="https://github.com/filipeleonelbatista/filipeleonelbatista/blob/master/assets/052-linkedin.svg" />
  </a>&ensp;
  <a href="mailto:filipe.x2016@gmail.com">
    <img alt="Email" width="22px" src="https://github.com/filipeleonelbatista/filipeleonelbatista/blob/master/assets/gmail.svg" />
  </a>&ensp;
  <a href="https://instagram.com/filipegaucho22">
    <img alt="Instagram" width="22px" src="https://github.com/filipeleonelbatista/filipeleonelbatista/blob/master/assets/044-instagram.svg" />
  </a>
</p>
<br />
<p align="center">
    Desenvolvido 💜 por Filipe Batista 
</p>
