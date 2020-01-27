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