let Module = null;
let mazePtr = null;
let filas = 0;
let cols = 0;
let jugadorRow = 0;
let jugadorCol = 0;
let terminado = false;
let visitadas = new Set(); // para pintar el camino recorrido, equivalente a on_solution

// Funciones de C envueltas para JS (concepto ya visto con cwrap)
let f_maze_create, f_is_wall, f_get_rows, f_get_cols,
    f_entry_row, f_entry_col, f_exit_row, f_exit_col,
    f_wall_north, f_wall_south, f_wall_east, f_wall_west;

const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');
const mensajeDiv = document.getElementById('mensaje');

MazeModule().then((mod) => {
    Module = mod;

    // Igual que en test.js, pero ahora con todas las funciones que necesitamos
    f_maze_create = Module.cwrap('maze_create', 'number', ['number', 'number']);
    f_is_wall     = Module.cwrap('is_Wall', 'number', ['number','number','number','number','number']);
    f_get_rows    = Module.cwrap('web_get_rows', 'number', ['number']);
    f_get_cols    = Module.cwrap('web_get_cols', 'number', ['number']);
    f_entry_row   = Module.cwrap('web_get_entry_row', 'number', ['number']);
    f_entry_col   = Module.cwrap('web_get_entry_col', 'number', ['number']);
    f_exit_row    = Module.cwrap('web_get_exit_row', 'number', ['number']);
    f_exit_col    = Module.cwrap('web_get_exit_col', 'number', ['number']);
    f_wall_north  = Module.cwrap('web_wall_north', 'number', ['number','number','number']);
    f_wall_south  = Module.cwrap('web_wall_south', 'number', ['number','number','number']);
    f_wall_east   = Module.cwrap('web_wall_east', 'number', ['number','number','number']);
    f_wall_west   = Module.cwrap('web_wall_west', 'number', ['number','number','number']);

    nuevoLaberinto(); // arranca con un maze apenas carga la página
});

function nuevoLaberinto(){
    const tamano = parseInt(document.getElementById('tamano').value);

    mazePtr = f_maze_create(tamano, tamano);
    filas = f_get_rows(mazePtr);
    cols = f_get_cols(mazePtr);
    jugadorRow = f_entry_row(mazePtr);
    jugadorCol = f_entry_col(mazePtr);
    terminado = false;
    visitadas = new Set();
    visitadas.add(`${jugadorRow},${jugadorCol}`);

    const gifVictoria = document.getElementById('gifVictoria');

    const tamanoGif = Math.floor(Math.random() * 101) + 300;
    console.log('Tamaño del GIF:', tamanoGif);
    gifVictoria.style.width = tamanoGif + 'px';
    gifVictoria.style.display = 'none';

    mensajeDiv.textContent = '';
    dibujarMaze();
}

function dibujarMaze(){
    const cellSize = Math.min(canvas.width / cols, canvas.height / filas);

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.strokeStyle = '#000';
    ctx.lineWidth = 2;

    for(let row = 0; row < filas; row++){
        for(let col = 0; col < cols; col++){
            const x = col * cellSize;
            const y = row * cellSize;

            // Camino recorrido (equivalente a on_solution)
            if(visitadas.has(`${row},${col}`)){
                ctx.fillStyle = '#ccffd0';
                ctx.fillRect(x, y, cellSize, cellSize);
            }

            ctx.beginPath();
            if(f_wall_north(mazePtr, row, col)){
                ctx.moveTo(x, y);
                ctx.lineTo(x + cellSize, y);
            }
            if(f_wall_south(mazePtr, row, col)){
                ctx.moveTo(x, y + cellSize);
                ctx.lineTo(x + cellSize, y + cellSize);
            }
            if(f_wall_east(mazePtr, row, col)){
                ctx.moveTo(x + cellSize, y);
                ctx.lineTo(x + cellSize, y + cellSize);
            }
            if(f_wall_west(mazePtr, row, col)){
                ctx.moveTo(x, y);
                ctx.lineTo(x, y + cellSize);
            }
            ctx.stroke();
        }
    }

    // Entrada (E) y salida (S)
    ctx.fillStyle = 'green';
    ctx.font = `${cellSize * 0.5}px sans-serif`;
    const er = f_entry_row(mazePtr), ec = f_entry_col(mazePtr);
    ctx.fillText('E', ec * cellSize + cellSize*0.25, er * cellSize + cellSize*0.7);

    ctx.fillStyle = 'red';
    const xr = f_exit_row(mazePtr), xc = f_exit_col(mazePtr);
    ctx.fillText('S', xc * cellSize + cellSize*0.25, xr * cellSize + cellSize*0.7);

    // Jugador
    ctx.fillStyle = 'blue';
    ctx.beginPath();
    ctx.arc(
        jugadorCol * cellSize + cellSize/2,
        jugadorRow * cellSize + cellSize/2,
        cellSize/4, 0, Math.PI*2
    );
    ctx.fill();
}

function moverJugador(dr, dc){
    if(terminado) return;

    const nuevaRow = jugadorRow + dr;
    const nuevaCol = jugadorCol + dc;

    if(nuevaRow < 0 || nuevaRow >= filas || nuevaCol < 0 || nuevaCol >= cols){
        mensajeDiv.textContent = 'No hay nada en esa dirección.';
        return;
    }

    if(f_is_wall(mazePtr, jugadorRow, jugadorCol, dr, dc)){
        mensajeDiv.textContent = 'Movimiento inválido: hay una pared.';
        return;
    }

    jugadorRow = nuevaRow;
    jugadorCol = nuevaCol;
    visitadas.add(`${jugadorRow},${jugadorCol}`);
    mensajeDiv.textContent = '';

    const xr = f_exit_row(mazePtr), xc = f_exit_col(mazePtr);
    if(jugadorRow === xr && jugadorCol === xc){
        terminado = true;

        mensajeDiv.textContent = '🎉 ¡GANASTE! 🏆 ¡Completaste el laberinto! 🎉';

        const gifVictoria = document.getElementById('gifVictoria');

        gifVictoria.style.display = 'block';
        gifVictoria.style.animation = 'aparecerVictoria 0.6s ease-out';
    }

    dibujarMaze();
}

document.addEventListener('keydown', (e) => {
    switch(e.key){
        case 'ArrowUp':    moverJugador(-1, 0); break;
        case 'ArrowDown':  moverJugador(1, 0);  break;
        case 'ArrowLeft':  moverJugador(0, -1); break;
        case 'ArrowRight': moverJugador(0, 1);  break;
    }
});

document.getElementById('btnNuevo').addEventListener('click', nuevoLaberinto);