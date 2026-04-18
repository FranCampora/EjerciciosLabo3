#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H

int abrirArchivo(char *path, char *modo);
void cerrarArchivo();
int escribirArchivo(char *texto);
int leerLinea(char *buffer);

int existe(char *path);
int contarLineas(char *path);
int renombrarArchivo(char *oldName, char *newName);
int borrarArchivo(char *path);

#endif

