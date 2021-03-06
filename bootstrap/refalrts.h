#ifndef RefalRTS_H_
#define RefalRTS_H_

namespace refalrts {

typedef enum FnResult {
  cRecognitionImpossible = 0,
  cNoMemory = 1,
  cSuccess = 2,
  cExit = 3
} FnResult;

typedef struct Node Node;

typedef struct Node *NodePtr;

typedef struct Node *Iter;

typedef enum DataTag {
  cDataIllegal = 0,
  cDataSwapHead,
  cDataChar,
  cDataNumber,
  cDataFunction,
  cDataIdentifier,
  cDataOpenADT, cDataCloseADT,
  cDataOpenBracket, cDataCloseBracket,
  cDataOpenCall, cDataCloseCall,
  cDataFile,
  cDataClosure,
  cDataUnwrappedClosure,
  cDataClosureHead
} DataTag;

typedef FnResult (*RefalFunctionPtr) ( Iter begin, Iter end );

typedef struct RefalFunction {
  RefalFunctionPtr ptr;
  const char *name;
} RefalFunction;

typedef unsigned long RefalNumber;

typedef const char *(*RefalIdentifier) ();

typedef struct RefalSwapHead {
  Iter next_head;
  const char *name;
} RefalSwapHead;

typedef struct Node {
  NodePtr prev;
  NodePtr next;
  DataTag tag;
  union {
    char char_info;
    RefalNumber number_info;
    RefalFunction function_info;
    RefalIdentifier ident_info;
    NodePtr link_info;
    void *file_info;
    RefalSwapHead swap_info;
  };
} Node;

typedef enum iCmd {
  icChar,
  icInt,
  icFunc,
  icIdent,
  icString,
  icBracket,
  icSpliceSTVar,
  icSpliceEVar,
  icCopySTVar,
  icCopyEVar,
  icEnd
} iCmd;

typedef enum BracketType {
  ibOpenADT,
  ibOpenBracket,
  ibOpenCall,
  ibCloseADT,
  ibCloseBracket,
  ibCloseCall
} BracketType;

typedef struct ResultAction {
    iCmd cmd;
    void *ptr_value1;
    void *ptr_value2;
    int value;
} ResultAction;

extern void use( Iter& );

// �������� ������������� �������

extern void move_left( Iter& begin, Iter& end );
extern void move_right( Iter& begin, Iter& end );
extern bool empty_seq( Iter begin, Iter end );

extern bool function_left( RefalFunctionPtr func, Iter& first, Iter& last );
extern bool function_right( RefalFunctionPtr func, Iter& first, Iter& last );

extern bool char_left( char ch, Iter& first, Iter& last );
extern bool char_right( char ch, Iter& first, Iter& last );

extern bool number_left( RefalNumber num, Iter& first, Iter& last );
extern bool number_right( RefalNumber num, Iter& first, Iter& last );

extern bool ident_left( RefalIdentifier ident, Iter& first, Iter& last );
extern bool ident_right( RefalIdentifier ident, Iter& first, Iter& last );

extern bool adt_left(
  Iter& res_first, Iter& res_last,
  RefalFunctionPtr tag,
  Iter& first, Iter& last
);
extern bool adt_right(
  Iter& res_first, Iter& res_last,
  RefalFunctionPtr tag,
  Iter& first, Iter& last
);

extern bool brackets_left( Iter& res_first, Iter& res_last, Iter& first, Iter& last );
extern bool brackets_right( Iter& res_first, Iter& res_last, Iter& first, Iter& last );

extern bool svar_left( Iter& svar, Iter& first, Iter& last );
extern bool svar_right( Iter& svar, Iter& first, Iter& last );

extern bool tvar_left( Iter& tvar, Iter& first, Iter& last );
extern bool tvar_right( Iter& tvar, Iter& first, Iter& last );

extern bool next_term( Iter& first, Iter& last );

extern bool repeated_stvar_left( Iter& stvar, Iter stvar_sample, Iter& first, Iter& last );
extern bool repeated_stvar_right( Iter& stvar, Iter stvar_sample, Iter& first, Iter& last );

extern bool repeated_evar_left(
  Iter& evar_b, Iter& evar_e,
  Iter evar_b_sample, Iter evar_e_sample,
  Iter& first, Iter& last
);

extern bool repeated_evar_right(
  Iter& evar_b, Iter& evar_e,
  Iter evar_b_sample, Iter evar_e_sample,
  Iter& first, Iter& last
);

extern unsigned read_chars(
  char buffer[], unsigned buflen, Iter& first, Iter& last
);

// �������� ���������� ����������

extern void reset_allocator();

extern bool copy_evar(
  Iter& evar_res_b, Iter& evar_res_e, Iter evar_b_sample, Iter evar_e_sample
);

extern bool copy_stvar( Iter& stvar_res, Iter stvar_sample );

extern bool alloc_char( Iter& res, char ch );
extern bool alloc_number( Iter& res, RefalNumber num );
extern bool alloc_name(
  Iter& res, RefalFunctionPtr func, const char *name = ""
);
extern bool alloc_ident( Iter& res, RefalIdentifier ident );
extern bool alloc_open_adt( Iter& res );
extern bool alloc_close_adt( Iter& res );
extern bool alloc_open_bracket( Iter& res );
extern bool alloc_close_bracket( Iter& res );
extern bool alloc_open_call( Iter& res );
extern bool alloc_close_call( Iter& res );

extern bool alloc_chars(
  Iter& res_b, Iter& res_e, const char buffer[], unsigned buflen
);
extern bool alloc_string( Iter& res_b, Iter& res_e, const char *string );

extern void push_stack( Iter call_bracket );
extern void link_brackets( Iter left, Iter right );

extern Iter splice_elem( Iter res, Iter elem );
extern Iter splice_stvar( Iter res, Iter var );
extern Iter splice_evar( Iter res, Iter first, Iter last );
extern void splice_to_freelist( Iter first, Iter last );

extern FnResult create_closure( Iter begin, Iter end );
Iter unwrap_closure( Iter closure ); // ���������� ���������
Iter wrap_closure( Iter closure ); // �������� ���������

// ������ �� ������������ �������

extern Iter initialize_swap_head( Iter head );
extern void swap_info_bounds( Iter& first, Iter& last, Iter head );
extern void swap_save( Iter head, Iter first, Iter last );

// ��������������

extern void this_is_generated_function();

// ������ �������

extern void set_return_code( int retcode );

// �������������

extern FnResult interpret_array(
  const ResultAction raa[],
  Iter allocs[],
  Iter begin,
  Iter end
);

} //namespace refalrts

#endif //RefalRTS_H_
