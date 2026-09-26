# Redes Neuronales Artificiales

Ejemplos didácticos de algoritmos de aprendizaje para Redes
Neuronales Artificiales.

## Ejemplos

- Regla de Hebb supervisada
- Regla Delta
- Backpropagation

Los ejemplos utilizan patrones de caracteres almacenados en:

- `Letras/TRAIN.DAT`
- `Letras/TEST.DAT`

## Requisitos

Se requiere un compilador de C++, por ejemplo GCC/G++.

Verificar la instalación:

```bash
g++ --version
```

## Compilacion
### Regla de Hebb
```bash
cd RNA_HEBB
g++ RNA_HEBB.cpp -o RNA_HEBB
```
### Regla Delta
```bash
cd RNA_DELTA
g++ RNA_DELTA.cpp -o RNA_DELTA
```
### Regla Delta
```bash
cd RNA_BACKPROP
g++ RNA_BACK.cpp -o RNA_BACK
```