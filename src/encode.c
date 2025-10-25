#include "encode.h"

int hufftree_compress(HuffTree *hf, FILE *f, int numbytes) {

    // position of the level order bytes
    long lob_pos = ftell(f);
    // position of the shape buffer
    long sb_pos = lob_pos + numbytes;

    // queue structure for level order traversal of the tree
    struct qel {
        struct qel *next;
        HuffTree *val;
    };
    struct qel *q = malloc(sizeof(struct qel));
    q->val = hf;
    q->next = NULL;
    struct qel *last = q;
    if (q == NULL) {
        perror("could not allocate enough memory");
        return 1;
    }

    BitWriter *sb_writer = BitWriter_create(f);
    unsigned char zero = 0;
    unsigned char one = UCHAR_MAX;

    while (q != NULL) {
        // if the current node is a leaf.
        if (q->val->left == NULL && q->val->right == NULL) {
            // write its value in level order buffer
            fseek(f, lob_pos, SEEK_SET);
            fputc(q->val->val, f);
            lob_pos++;
            // write 1 into the shape buffer
            fseek(f, sb_pos, SEEK_SET);
            BitWriter_write(sb_writer, &one, 1);
            sb_pos = ftell(f);
        }
        // current node is not a leaf
        else {
            // write 0 into the shape buffer
            fseek(f, sb_pos, SEEK_SET);
            BitWriter_write(sb_writer, &zero, 1);
            sb_pos = ftell(f);
            // add children to the end of the queue
            struct qel *l = malloc(sizeof(struct qel));
            struct qel *r = malloc(sizeof(struct qel));
            l->val = q->val->left;
            l->next = r;
            r->val = q->val->right;
            r->next = NULL;
            last->next = l;
            last = r;
        }
        // remove curr node from the queue
        struct qel *temp = q;
        q = q->next;
        free(temp);
    }
    BitWriter_free(&sb_writer);
    return 0;
}

int gen_encodings(HuffTree *ht, BitString **encoding, char *curr_str, int depth) {
    if (ht == NULL) return 1;
    if (ht->left==NULL && ht->right==NULL) {
        curr_str[depth] = '\0';
        printf("%c - %s\n", ht->val, curr_str);
        encoding[ht->val] = malloc(sizeof(BitString));
        if (encoding[ht->val] == NULL) {
            perror("could not allocate enough memory for bitstring");
            return 1;
        }
        int res = Bitstring_create(encoding[ht->val], curr_str);
        if (res != 0) {
            printf("%d\n", res);
            perror("something went wrong");
            return 1;
        }
    }
    else {
        curr_str[depth] = '0';
        int a = 0;
        a += gen_encodings(ht->left, encoding, curr_str, depth+1);
        curr_str[depth] = '1';
        a += gen_encodings(ht->right, encoding, curr_str, depth+1);
        return a;
    }
    return 0;
}

int filedata_compress(FILE* f, HuffTree *ht, FILE* outfile, int numbytes) {

    BitString **encoding = calloc(UCHAR_MAX+1, sizeof(BitString *));
    if (encoding == NULL) {
        perror("could not allocate enough memory");
        return 1;
    }

    // generate encodings for characters in hufftree
    char *str =  calloc(numbytes*2, 1);
    if (str == NULL) {
        perror("could not allocate enought memory");
        free(encoding);
    }
    int res = gen_encodings(ht, encoding, str, 0);
    free(str);
    if (res != 0) {
        perror("something went wrong while generating encodings");
        for (size_t i=0; i<UCHAR_MAX+1; i++) {
            Bitstring_free(encoding[i]);
        }
        free(encoding);
        return 1;
    }

    // save lboffset position for later
    long lboffset_pos = ftell(outfile);
    fseek(outfile, lboffset_pos + 1, SEEK_SET);

    // encode the f data and write it into outfile
    int ch;
    BitWriter *bw = BitWriter_create(outfile);
    while ((ch=fgetc(f)) != EOF) {
        BitWriter_write(bw, encoding[ch]->data, encoding[ch]->length);
    }
    
    // save lboffset
    fseek(outfile, lboffset_pos, SEEK_SET);
    fputc(bw->curr_bit, outfile);
    BitWriter_free(&bw);

    // free encoding data
    for (size_t i=0; i<UCHAR_MAX+1; i++) {
        Bitstring_free(encoding[i]);
    }
    free(encoding);
    return 0;
}

int compress(FILE *f, FILE *out_path)
{
    // STEP 1: get byte frequencies in f.
    ByteFrequencies *bf = ByteFrequencies_from_file(f);
    if (bf == NULL) {
        perror("could not create byte frequencies");
        return 1;
    }

    // number of distinct bytes in file. useful for later.
    unsigned char numbytes = 0;

    for (size_t i=0; i<UCHAR_MAX+1; i++) {
        if ((*bf)[i]>0) numbytes++;
    }

    // STEP 2: use byte frequencies to generate huffman tree.
    HuffTree *ht = HuffTree_create(bf);
    ByteFrequencies_free(&bf);
    if (ht == NULL) {
        perror("could not create huffman coding tree.");
        return 1;
    }

    // STEP 3: compress huffman tree and put it into file.
    // 3.1: put numbytes into file
    fputc(numbytes, out_path);
    // 3.2: put level order bytes and shape buffer into the file.
    hufftree_compress(ht, out_path, numbytes);

    // STEP 4: compress the file's data.
    filedata_compress(f, ht, out_path, numbytes);

    HuffTree_free(ht);
    return 0;
}