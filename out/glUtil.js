function sleep(ms) {
  return new Promise(r => setTimeout(r, ms));
}

function genCircle(radius, x, y, precision) {
  let arr = new Float32Array(3 * precision);
  genCircleInPlace(arr, radius, x, y);
  return arr;
}

function genCircleInPlace(arr, radius, x, y) {
  let precision = arr.length / 3 - 1;
  for (let j = 0; j <= precision; ++j) {
    arr[3 * j] = radius * Math.cos(2 * j * Math.PI / precision) + x;
    arr[3 * j + 1] = radius * Math.sin(2 * j * Math.PI / precision) + y;
  }
}

function genColor() {
  return [Math.random(), Math.random(), Math.random()];
}

/**
 * @return {WebGL2RenderingContex} context
 */
function getRenderingContext() {
  let canvas = document.getElementsByTagName('canvas')[0];
  let gl = canvas.getContext('webgl2');

  if (gl === null) {
    alert('Unable to initialize WebGL.');
    return null;
  }

  let shaderProgram =
    createShaderProgram(gl, vertexShaderSource, fragmentShaderSource);
  gl.useProgram(shaderProgram);

  return {
    gl: gl,
    shader: shaderProgram
  };

  /* let stop = false;
  document.getElementById('stop').addEventListener('click', function () {
    this.innerText = (stop = !stop) ? 'start' : 'stop';
  });

  const precision = 5;
  let vbos = [];
  document.getElementById('add').addEventListener('click', () => {
    const vbo = gl.createBuffer();
    vbos.push(vbo);
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      genCircle(0.1, Math.random() * 2 - 1, Math.random() * 2 - 1, precision),
      gl.STREAM_DRAW
    );
    document.getElementById('list').appendChild((() => {
      let p = document.createElement('p');
      p.innerText = '' + vbos.length - 1;
      return p;
    })());
  })

  document.getElementById('update').addEventListener('click', () => {
    gl.bindBuffer(gl.ARRAY_BUFFER, vbos[Number.parseInt(document.getElementById('id').value)]);
    gl.bufferData(
      gl.ARRAY_BUFFER,
      genCircle(
        0.1,
        Number.parseFloat(document.getElementById('x').value),
        Number.parseFloat(document.getElementById('y').value),
        precision),
      gl.STREAM_DRAW
    );
  });

  let iter = 0;
  while (true) {
    await sleep(1000 / 60);
    if (stop) {
      await sleep(200);
      continue;
    }
    gl.clearColor((Math.sin(iter++ / 50) + 1) / 3, 1.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);
    for (const vbo of vbos) {
      gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
      gl.vertexAttribPointer(0, 3, gl.FLOAT, false, 0, 0);
      gl.enableVertexAttribArray(0);
      gl.drawArrays(gl.TRIANGLE_FAN, 0, precision);
    }
  }
 */
  /* for (let i = 0; !stop; ++i) {
    await sleep(1000 / 60);
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.bindBuffer(gl.ARRAY_BUFFER, vbo);
    genCircleInPlace(circle, Math.sin(i / 50) / 2 + 0.5, 0, 0);
    gl.bufferSubData(gl.ARRAY_BUFFER, 0, circle);

    gl.drawArrays(gl.TRIANGLE_FAN, 0, precision);
  } */
}

// raw shaders

let vertexShaderSource = `
  #version 300 es

  layout (location = 0) in vec3 pos;

  void main() {
    gl_Position = vec4(pos, 1.0);
  }
`.trim();

let fragmentShaderSource = `
  #version 300 es

  uniform highp vec3 customColor;
  out highp vec4 color;

  void main() {
    color = vec4(customColor, 1.0);
  }
`.trim();

// shader utility

/**
 * @param {WebGLRenderingContext} gl 
 * @param {*} type 
 * @param {*} source 
 * 
 * @return {WebGLShader}
 */
function loadShader(gl, type, source) {
  let shader = gl.createShader(type);

  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    alert('Error while compiling shader: ' + gl.getShaderInfoLog(shader));
    gl.deleteShader(shader);
    return null;
  }

  return shader;
}

/**
 * @param {WebGLRenderingContext} gl 
 * @param {*} vertexShader 
 * @param {*} fragmentShader 
 * 
 * @return {WebGLProgram}
 */
function createShaderProgram(gl, vSrc, fSrc) {
  let vertexShader = loadShader(gl, gl.VERTEX_SHADER, vSrc);
  let fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fSrc);

  let program = gl.createProgram();

  gl.attachShader(program, vertexShader);
  gl.attachShader(program, fragmentShader);

  gl.linkProgram(program);

  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    alert('Error while linking the shader program: '
      + gl.getProgramInfoLog(program));
    return null;
  }

  return program;
}