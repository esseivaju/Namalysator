// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#ifndef CXX___BNL_NEWSPAPER_V1_3_PIMPL_HXX
#define CXX___BNL_NEWSPAPER_V1_3_PIMPL_HXX

#include "bnl-newspaper_v1.3-pskel.hxx"

class ct_HIERARCHY_pimpl: public virtual ct_HIERARCHY_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  SUPPLEMENT ();

  virtual void
  ISSUE ();

  virtual void
  post_ct_HIERARCHY ();
};

class ct_ISSUE_pimpl: public virtual ct_ISSUE_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TITLE_SECTION ();

  virtual void
  CONTENT ();

  virtual void
  SUPPLEMENT ();

  virtual void
  post_ct_ISSUE ();
};

class ct_SUPPLEMENT_pimpl: public virtual ct_SUPPLEMENT_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TITLE_SECTION ();

  virtual void
  CONTENT ();

  virtual void
  post_ct_SUPPLEMENT ();
};

class ct_LIST_LIKE_pimpl: public virtual ct_LIST_LIKE_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  CAPTION ();

  virtual void
  DATA ();

  virtual void
  TABLE_OF_CONTENTS ();

  virtual void
  LIST ();

  virtual void
  ITEM ();

  virtual void
  post_ct_LIST_LIKE ();
};

class ct_ITEM_pimpl: public virtual ct_ITEM_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  PAGE_NUMBER ();

  virtual void
  ITEM_CAPTION ();

  virtual void
  SUBHEADLINE ();

  virtual void
  OVERLINE ();

  virtual void
  HEADLINE ();

  virtual void
  CONTINUATION_HEADLINE ();

  virtual void
  AUTHOR ();

  virtual void
  ITEM ();

  virtual void
  post_ct_ITEM ();
};

class ct_SECTION_pimpl: public virtual ct_SECTION_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  HEADING ();

  virtual void
  BODY ();

  virtual void
  SECTION ();

  virtual void
  NECROLOGY ();

  virtual void
  WEATHER ();

  virtual void
  SHIPPING ();

  virtual void
  RIDAS ();

  virtual void
  LETTER ();

  virtual void
  INTERVIEW ();

  virtual void
  IMPRINT ();

  virtual void
  ARTICLE ();

  virtual void
  ACKNOWLEDGEMENT ();

  virtual void
  post_ct_SECTION ();
};

class ct_CHAPTER_pimpl: public virtual ct_CHAPTER_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  HEADING ();

  virtual void
  BODY ();

  virtual void
  LIST ();

  virtual void
  CHAPTER ();

  virtual void
  post_ct_CHAPTER ();
};

class ct_ARTICLE_LIKE_pimpl: public virtual ct_ARTICLE_LIKE_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  HEADING ();

  virtual void
  BODY ();

  virtual void
  TABLE_OF_CONTENTS ();

  virtual void
  LIST ();

  virtual void
  CHAPTER ();

  virtual void
  post_ct_ARTICLE_LIKE ();
};

class ct_CHAPTER_LIKE_HEADING_pimpl: public virtual ct_CHAPTER_LIKE_HEADING_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  OVERLINE ();

  virtual void
  TITLE ();

  virtual void
  SUBHEADLINE ();

  virtual void
  AUTHOR ();

  virtual void
  TEXT ();

  virtual void
  post_ct_CHAPTER_LIKE_HEADING ();
};

class ct_CHAPTER_HEADING_pimpl: public virtual ct_CHAPTER_HEADING_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TITLE ();

  virtual void
  AUTHOR ();

  virtual void
  TEXT ();

  virtual void
  post_ct_CHAPTER_HEADING ();
};

class ct_ILLUSTRATION_LIKE_STRUCT_pimpl: public virtual ct_ILLUSTRATION_LIKE_STRUCT_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  IMAGE ();

  virtual void
  CAPTION ();

  virtual void
  AUTHOR ();

  virtual void
  post_ct_ILLUSTRATION_LIKE_STRUCT ();
};

class ct_TABLE_LIKE_STRUCT_pimpl: public virtual ct_TABLE_LIKE_STRUCT_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  IMAGE ();

  virtual void
  CAPTION ();

  virtual void
  post_ct_TABLE_LIKE_STRUCT ();
};

class ct_BODY_ARTICLE_pimpl: public virtual ct_BODY_ARTICLE_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  BODY_CONTENT ();

  virtual void
  FOOTNOTE ();

  virtual void
  ILLUSTRATION ();

  virtual void
  TABLE ();

  virtual void
  post_ct_BODY_ARTICLE ();
};

class ct_BODY_CHAPTER_pimpl: public virtual ct_BODY_CHAPTER_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  BODY_CONTENT ();

  virtual void
  FOOTNOTE ();

  virtual void
  ILLUSTRATION ();

  virtual void
  TABLE ();

  virtual void
  post_ct_BODY_CHAPTER ();
};

class ct_BODY_SECTION_pimpl: public virtual ct_BODY_SECTION_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  BODY_CONTENT ();

  virtual void
  FOOTNOTE ();

  virtual void
  ILLUSTRATION ();

  virtual void
  TABLE ();

  virtual void
  post_ct_BODY_SECTION ();
};

class ct_BODY_CONTENT_ARTICLE_pimpl: public virtual ct_BODY_CONTENT_ARTICLE_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  AUTHOR ();

  virtual void
  PARAGRAPH ();

  virtual void
  LINK ();

  virtual void
  FORMULA ();

  virtual void
  CROSSHEADING ();

  virtual void
  CONTINUATION_LINK ();

  virtual void
  CONTINUATION_HEADLINE ();

  virtual void
  ABSTRACT ();

  virtual void
  post_ct_BODY_CONTENT_ARTICLE ();
};

class ct_BODY_CONTENT_CHAPTER_pimpl: public virtual ct_BODY_CONTENT_CHAPTER_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  PARAGRAPH ();

  virtual void
  LINK ();

  virtual void
  FORMULA ();

  virtual void
  CROSSHEADING ();

  virtual void
  CONTINUATION_LINK ();

  virtual void
  CONTINUATION_HEADLINE ();

  virtual void
  post_ct_BODY_CONTENT_CHAPTER ();
};

class ct_BODY_CONTENT_SECTION_pimpl: public virtual ct_BODY_CONTENT_SECTION_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  PARAGRAPH ();

  virtual void
  MARRIAGE ();

  virtual void
  DEATH_NOTICE ();

  virtual void
  BIRTH ();

  virtual void
  ADVERTISEMENT ();

  virtual void
  post_ct_BODY_CONTENT_SECTION ();
};

class ct_FORMULA_pimpl: public virtual ct_FORMULA_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  IMAGE ();

  virtual void
  post_ct_FORMULA ();
};

class ct_PARAGRAPH_pimpl: public virtual ct_PARAGRAPH_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXT ();

  virtual void
  post_ct_PARAGRAPH ();
};

class ct_FOOTNOTE_PARAGRAPH_pimpl: public virtual ct_FOOTNOTE_PARAGRAPH_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXT ();

  virtual void
  post_ct_FOOTNOTE_PARAGRAPH ();
};

class ct_FINAL_pimpl: public virtual ct_FINAL_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  fptr ();

  virtual void
  post_ct_FINAL ();
};

class ct_FPTR_pimpl: public virtual ct_FPTR_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  area ();

  virtual void
  seq ();

  virtual void
  par ();

  virtual void
  post_ct_FPTR ();
};

class ct_SEQ_pimpl: public virtual ct_SEQ_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  area ();

  virtual void
  post_ct_SEQ ();
};

class Newspaper_pimpl: public virtual Newspaper_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  VOLUME ();

  virtual void
  post_Newspaper ();
};

class TITLE_SECTION_pimpl: public virtual TITLE_SECTION_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  TABLE_OF_CONTENTS ();

  virtual void
  PAGE_NUMBER ();

  virtual void
  ILLUSTRATION ();

  virtual void
  SUBHEADLINE ();

  virtual void
  OVERLINE ();

  virtual void
  HEADLINE ();

  virtual void
  MOTTO ();

  virtual void
  CONTINUATION_HEADLINE ();

  virtual void
  post_TITLE_SECTION ();
};

class CONTENT_pimpl: public virtual CONTENT_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  SECTION ();

  virtual void
  PARAGRAPH ();

  virtual void
  NECROLOGY ();

  virtual void
  WEATHER ();

  virtual void
  SHIPPING ();

  virtual void
  RIDAS ();

  virtual void
  LETTER ();

  virtual void
  INTERVIEW ();

  virtual void
  IMPRINT ();

  virtual void
  ARTICLE ();

  virtual void
  ACKNOWLEDGEMENT ();

  virtual void
  MARRIAGE ();

  virtual void
  DEATH_NOTICE ();

  virtual void
  BIRTH ();

  virtual void
  ADVERTISEMENT ();

  virtual void
  post_CONTENT ();
};

class TITLE_SECTION1_pimpl: public virtual TITLE_SECTION1_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  TABLE_OF_CONTENTS ();

  virtual void
  PAGE_NUMBER ();

  virtual void
  ILLUSTRATION ();

  virtual void
  SUBHEADLINE ();

  virtual void
  OVERLINE ();

  virtual void
  HEADLINE ();

  virtual void
  MOTTO ();

  virtual void
  CONTINUATION_HEADLINE ();

  virtual void
  post_TITLE_SECTION1 ();
};

class CONTENT1_pimpl: public virtual CONTENT1_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  SECTION ();

  virtual void
  PARAGRAPH ();

  virtual void
  NECROLOGY ();

  virtual void
  WEATHER ();

  virtual void
  SHIPPING ();

  virtual void
  RIDAS ();

  virtual void
  LETTER ();

  virtual void
  INTERVIEW ();

  virtual void
  IMPRINT ();

  virtual void
  ARTICLE ();

  virtual void
  ACKNOWLEDGEMENT ();

  virtual void
  MARRIAGE ();

  virtual void
  DEATH_NOTICE ();

  virtual void
  BIRTH ();

  virtual void
  ADVERTISEMENT ();

  virtual void
  post_CONTENT1 ();
};

class DATA_pimpl: public virtual DATA_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  TEXTBLOCK ();

  virtual void
  PUBLISHING_STMT ();

  virtual void
  PAGE_NUMBER ();

  virtual void
  ITEM_CAPTION ();

  virtual void
  AUTHOR ();

  virtual void
  post_DATA ();
};

class par_pimpl: public virtual par_pskel
{
  public:
  virtual void
  pre ();

  virtual void
  area ();

  virtual void
  seq ();

  virtual void
  post_par ();
};

#endif // CXX___BNL_NEWSPAPER_V1_3_PIMPL_HXX
