const MazeModule = require('./wasm/maze.js');

MazeModule().then((Module) => {
    const maze_create = Module.cwrap('maze_create', 'number', ['number', 'number']);
    const web_get_rows = Module.cwrap('web_get_rows', 'number', ['number']);
    const web_wall_north = Module.cwrap('web_wall_north', 'number', ['number', 'number', 'number']);

    const mazePtr = maze_create(5, 5);
    console.log('Puntero al maze:', mazePtr);
    console.log('Filas:', web_get_rows(mazePtr));
    console.log('¿Pared norte en (0,0)?:', web_wall_north(mazePtr, 0, 0));
});