#ifndef _ARCHIVOS_H
#define _ARCHIVOS_H

int abrirArchivo(char *path, char *modo);
int cerrarArchivo();
int leerArchivo(char *path);
int escribirArchivo(char *path, char *modo, char *string);
int escribirLinea(char *string);
char* leerLinea();
void archivoInicial();

int existe(char *path);
int contarLineas(char *path);
int renombrarArchivo(char *oldName, char *newName);
int borrarArchivo(char *path);
int leerUltimaLinea(char *path, char *modo, char *ultimaLinea);

#endif

