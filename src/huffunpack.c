#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cwalk.h> // Per la gestione dei percorsi

#include "decode.h" // Per la funzione di decompressione

int main(int argc, char *argv[]) {

    // Buffer per la directory di output, normalizzato
    char outdir_buf[FILENAME_MAX] = "./"; 
    char *outdir = outdir_buf; 
    int outdir_specified = 0;
    const char ext[] = ".huff"; // Estensione che cerchiamo

    // --- Gestione --help ---
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        // Assumiamo che esista un file di aiuto per huffunpack
        FILE *help = fopen("./misc/huffunpack_help.txt", "rb");
        if (help == NULL) {
            puts("missing help file");
            return 1;
        }
        int ch;
        while ((ch = fgetc(help)) != EOF) {
            putchar(ch);
        }
        fclose(help);
        return 0;
    }

    // --- Loop sugli argomenti ---
    for (int i = 1; i < argc; i++) {

        // --- Gestione -o ---
        if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i == argc) {
                puts("dir name must be specified for optional parameter -o");
                exit(1);
            }
            if (outdir_specified == 1) {
                puts("output directory specified more than once");
                exit(1);
            }
            outdir_specified = 1;

            // Copia e normalizza il percorso di output
            strncpy(outdir_buf, argv[i], sizeof(outdir_buf) - 1);
            outdir_buf[sizeof(outdir_buf) - 1] = '\0'; 
            cwk_path_normalize(outdir_buf, outdir_buf, sizeof(outdir_buf));
            
            continue;
        }

        // --- Gestione File di Input ---

        // 1. Controllo estensione del file di input
        const char *input_ext;
        size_t input_ext_len;
        cwk_path_get_extension(argv[i], &input_ext, &input_ext_len);

        if (input_ext == NULL || strcmp(input_ext, ext) != 0) {
            fprintf(stderr, "ERRORE: Il file %s non ha estensione %s. Ignorato.\n", argv[i], ext);
            continue; // Salta al prossimo file
        }

        // 2. Apri il file di input
        FILE *in = fopen(argv[i], "rb");
        if (in == NULL) {
            perror("could not open file");
            continue;
        }

        // 3. Costruzione percorso di output (usando cwalk)
        const char *basename;
        size_t basename_len;
        char basename_nullterm[FILENAME_MAX];
        char joined_path[FILENAME_MAX];
        char final_out_path[FILENAME_MAX];

        // 3a. Estrai il basename (es. "file.huff")
        cwk_path_get_basename(argv[i], &basename, &basename_len);

        // 3b. Copia e termina il basename
        if (basename_len >= sizeof(basename_nullterm)) {
            fprintf(stderr, "ERRORE: filename is too long %s\n", argv[i]);
            fclose(in);
            continue;
        }
        strncpy(basename_nullterm, basename, basename_len);
        basename_nullterm[basename_len] = '\0';

        // 3c. Unisci outdir (es. "build/") e basename (es. "file.huff")
        // Risultato: "build/file.huff"
        cwk_path_join(outdir, basename_nullterm, joined_path, sizeof(joined_path));

        // 3d. Cambia l'estensione da ".huff" a "" (niente)
        // Risultato: "build/file"
        cwk_path_change_extension(joined_path, "", final_out_path, sizeof(final_out_path));

        char *last_ext = NULL;
        char *curr = final_out_path;
        while (*curr != '\0') {
            if (*(curr) == '.') {
                last_ext = curr;
            }
            curr++;
        }
        if (last_ext != NULL) {
            *last_ext = '\0';
        }

        // 4. Apri il file di output
        FILE *out = fopen(final_out_path, "wb");

        if (out == NULL) {
            fprintf(stderr, "ERRORE: could not create file %s\n", final_out_path);
            fclose(in);
            continue;
        } 
        
        // 5. Decomprimi
        if (decode(in, out) == 0) { // Assumo che decode() restituisca 0 in caso di successo
            printf("Decompresso %s in %s\n", argv[i], final_out_path);
        } else {
            fprintf(stderr, "ERRORE: la decompressione di %s in %s è fallita\n", argv[i], final_out_path);
        }

        fclose(in);
        fclose(out);
    }

    return 0;
}