#include "decode.h"


HuffTree *HuffTree_from_file(FILE *src) {
    HuffTree *result = calloc(1, sizeof(HuffTree));
    if (result == NULL) {
        perror("could not allocate enough memory");
        return NULL;
    }

    // create a queue data structure for generating the resulting hufftree
    struct q {
        struct q *next;
        HuffTree *val;
    };
    struct q *Q = NULL; // queue used for iteration
    struct q *Q_last = NULL; // pointer to last element of Q

    #define QFREE while (Q != NULL) { \
        struct q* curr = Q;  \
        Q = Q->next; \
        free(curr); \
    }

    #define QENQUEUE(q) do { \
        if (Q_last != NULL) { \
            Q_last->next = (q); \
            Q_last = Q_last->next; \
        } \
        else { \
            Q = (q); \
            Q_last = (q); \
        } \
    } while(0);

    // initialize Q
    {
        struct q *initial_q = calloc(1, sizeof(struct q));
        if (initial_q == NULL) {
            perror("could not allocate enough memory");
            HuffTree_free(result);
            return NULL;
        }
        initial_q->val = result;
        QENQUEUE(initial_q);
    }

    // numbytes, position of the level order bytes and shape buffer
    int numbytes = fgetc(src) + 1;
    long lob_pos = 1;
    long sb_pos = 1 + numbytes;

    // bit reader used to read from shape buffer
    fseek(src, sb_pos, SEEK_SET);
    BitReader *br = BitReader_create(src);
    if (br == NULL) {
        perror("could not allocate enough memory");
        QFREE;
        HuffTree_free(result);
        return NULL;
    }

    while (Q != NULL) {
        // get curr value from queue and pop the first element
        HuffTree *curr = Q->val;
        {
            struct q *curr_q = Q;
            Q = Q->next;
            free(curr_q);
            if (Q == NULL) 
                Q_last = NULL;
        }

        // read next bit with br
        int bit = BitReader_read(br);

        // if the current bit read is 1, the current node must be a leaf,
        // so update its val field and leave its children as null
        if (bit == 1) {
            // get value of current node
            sb_pos = ftell(src);
            fseek(src, lob_pos, SEEK_SET);
            unsigned char val = fgetc(src);
            lob_pos = ftell(src);
            // set its val field to val
            curr->val = val;
            fseek(src, sb_pos, SEEK_SET);
        }

        // if the current bit read is 0, the current node is internal.
        // add its children to the end of the queue
        else if (bit == 0){
            struct q *left, *right;
            HuffTree *lval, *rval;
            left  = calloc(1, sizeof(struct q));
            right = calloc(1, sizeof(struct q));
            lval  = calloc(1, sizeof(HuffTree));
            rval  = calloc(1, sizeof(HuffTree));
            if (left==NULL || right==NULL || lval==NULL || rval==NULL ) {
                free(left); free(right); free(lval); free(rval);
                QFREE;
                HuffTree_free(result);
                perror("could not allocate enough memory");
                return NULL;
            }
            // set lval and rval to left and right children of hufftree curr
            curr->left  = lval;
            curr->right = rval;
            // set lval and rval to values of left and right
            left->val  = lval;
            right->val = rval;
            // enqueue left and right
            QENQUEUE(left);
            QENQUEUE(right);
        }

        // if current bit read is different from 0 or 1 something went wrong
        else {
            perror("something went wrong while reading file");
            QFREE;
            HuffTree_free(result);
            return NULL;
        }
    }

    BitReader_free(&br);

    #undef QFREE
    #undef QENQUEUE

    return result;
}


int decode(FILE *src, FILE *dest) {

    // check if src size is 0 (in that case do nothing) 
    {
        fseek(src, 0, SEEK_END);
        long bytes = ftell(src);
        fseek(src, 0, SEEK_SET);
        if (bytes == 0) {
            return 0;
        }
    }

    // generate hufftree from src header
    HuffTree *hf = HuffTree_from_file(src);
    if (hf == NULL) {
        perror("something went wrong");
        return -1;
    }

    // get last byte's offset.
    unsigned char lboffset = fgetc(src);

    // decode compressed data and write it into dest
    BitReader *br = BitReader_create(src);
    long bytes_until_end;
    {
        long curr_pos = ftell(src);
        fseek(src, 0, SEEK_END);
        long end_pos = ftell(src);
        bytes_until_end = end_pos - curr_pos;
        fseek(src, curr_pos, SEEK_SET);
    }

    HuffTree *curr = hf;
    #define ISLEAF  (curr->left==NULL && curr->right==NULL)

    while (
           (bytes_until_end>0) || 
           (bytes_until_end==0 && br->curr_bit != lboffset)
        ) {
        // check if curr node is leaf
        if (ISLEAF) {
            fputc(curr->val, dest);
            curr = hf;
            continue;
        }
        // check if a new byte is going to be read by br
        if (br->curr_bit==0) {
            bytes_until_end--;
        } 
        int bit = BitReader_read(br);

        switch(bit) {
            case 0:
                if (curr->left == NULL) goto decoding_error;
                curr = curr->left;
                break;
            case 1:
                if (curr->right == NULL) goto decoding_error;
                curr = curr->right;
                break;
            default:
                goto decoding_error;
                break;
        }
    }

    if (ISLEAF) {
        fputc(curr->val, dest);
    }

    #undef ISLEAF

    BitReader_free(&br);
    HuffTree_free(hf);
    return 0;

decoding_error:
    perror("something went wrong while decoding");
    BitReader_free(&br);
    HuffTree_free(hf);
    return -1;
}