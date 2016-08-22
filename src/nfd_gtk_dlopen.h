#ifndef NFD_GTK_DLOPEN_H
#define NFD_GTK_DLOPEN_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/* From glib.h */

/* Provide type definitions for commonly used types.
 *  These are useful because a "gint8" can be adjusted
 *  to be 1 byte (8 bits) on all platforms. Similarly and
 *  more importantly, "gint32" can be adjusted to be
 *  4 bytes (32 bits) on all platforms.
 */

typedef char   gchar;
typedef short  gshort;
typedef long   glong;
typedef int    gint;
typedef gint   gboolean;

typedef unsigned char	guchar;
typedef unsigned short	gushort;
typedef unsigned long	gulong;
typedef unsigned int	guint;

typedef float	gfloat;
typedef double	gdouble;


typedef void* gpointer;
typedef const void *gconstpointer;

/* From a glibconfig.h */
#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64)

#define GLIB_SIZEOF_VOID_P 8
#define GLIB_SIZEOF_LONG   4
#define GLIB_SIZEOF_SIZE_T 8

typedef signed long long gssize;
typedef unsigned long long gsize;

#else

#define GLIB_SIZEOF_VOID_P 4
#define GLIB_SIZEOF_LONG   4
#define GLIB_SIZEOF_SIZE_T 4

typedef signed int gssize;
typedef unsigned int gsize;

#endif
#define G_GNUC_EXTENSION __extension__
#define G_GNUC_CONST
#define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))


/* From gtype.h */
/* Typedefs
 */
#if     GLIB_SIZEOF_SIZE_T != GLIB_SIZEOF_LONG || !defined __cplusplus
typedef gsize                           GType;
#else   /* for historic reasons, C++ links against gulong GTypes */
typedef gulong                          GType;
#endif

typedef struct _GValue                  GValue;
typedef union  _GTypeCValue             GTypeCValue;
typedef struct _GTypePlugin             GTypePlugin;
typedef struct _GTypeClass              GTypeClass;
typedef struct _GTypeInterface          GTypeInterface;
typedef struct _GTypeInstance           GTypeInstance;
typedef struct _GTypeInfo               GTypeInfo;
typedef struct _GTypeFundamentalInfo    GTypeFundamentalInfo;
typedef struct _GInterfaceInfo          GInterfaceInfo;
typedef struct _GTypeValueTable         GTypeValueTable;
typedef struct _GTypeQuery		GTypeQuery;


/* Basic Type Structures
 */
struct _GTypeClass
{
  /*< private >*/
  GType g_type;
};
struct _GTypeInstance
{
  /*< private >*/
  GTypeClass *g_class;
};
struct _GTypeInterface
{
  /*< private >*/
  GType g_type;         /* iface type */
  GType g_instance_type;
};
struct _GTypeQuery
{
  GType		type;
  const gchar  *type_name;
  guint		class_size;
  guint		instance_size;
};


/* Casts, checks and accessors for structured types
 * usage of these macros is reserved to type implementations only
 */
/*< protected >*/
#define G_TYPE_CHECK_INSTANCE(instance)				(_G_TYPE_CHI ((GTypeInstance*) (instance)))
#define G_TYPE_CHECK_INSTANCE_CAST(instance, g_type, c_type)    (_G_TYPE_CIC ((instance), (g_type), c_type))
#define G_TYPE_CHECK_INSTANCE_TYPE(instance, g_type)            (_G_TYPE_CIT ((instance), (g_type)))
#define G_TYPE_INSTANCE_GET_CLASS(instance, g_type, c_type)     (_G_TYPE_IGC ((instance), (g_type), c_type))
#define G_TYPE_INSTANCE_GET_INTERFACE(instance, g_type, c_type) (_G_TYPE_IGI ((instance), (g_type), c_type))
#define G_TYPE_CHECK_CLASS_CAST(g_class, g_type, c_type)        (_G_TYPE_CCC ((g_class), (g_type), c_type))
#define G_TYPE_CHECK_CLASS_TYPE(g_class, g_type)                (_G_TYPE_CCT ((g_class), (g_type)))
#define G_TYPE_CHECK_VALUE(value)				(_G_TYPE_CHV ((value)))
#define G_TYPE_CHECK_VALUE_TYPE(value, g_type)			(_G_TYPE_CVH ((value), (g_type)))
#define G_TYPE_FROM_INSTANCE(instance)                          (G_TYPE_FROM_CLASS (((GTypeInstance*) (instance))->g_class))
#define G_TYPE_FROM_CLASS(g_class)                              (((GTypeClass*) (g_class))->g_type)
#define G_TYPE_FROM_INTERFACE(g_iface)                          (((GTypeInterface*) (g_iface))->g_type)

#define G_TYPE_INSTANCE_GET_PRIVATE(instance, g_type, c_type)   ((c_type*) g_type_instance_get_private ((GTypeInstance*) (instance), (g_type)))


/* NOTE: I needed to disable the cast checks, otherwise the program always crashed. */
#define G_DISABLE_CAST_CHECKS

/* --- implementation bits --- */
#ifndef G_DISABLE_CAST_CHECKS
#  define _G_TYPE_CIC(ip, gt, ct) \
    ((ct*) g_type_check_instance_cast ((GTypeInstance*) ip, gt))
#  define _G_TYPE_CCC(cp, gt, ct) \
    ((ct*) g_type_check_class_cast ((GTypeClass*) cp, gt))
#else /* G_DISABLE_CAST_CHECKS */
#  define _G_TYPE_CIC(ip, gt, ct)       ((ct*) ip)
#  define _G_TYPE_CCC(cp, gt, ct)       ((ct*) cp)
#endif /* G_DISABLE_CAST_CHECKS */
#define _G_TYPE_CHI(ip)			(g_type_check_instance ((GTypeInstance*) ip))
#define _G_TYPE_CHV(vl)			(g_type_check_value ((GValue*) vl))
#define _G_TYPE_IGC(ip, gt, ct)         ((ct*) (((GTypeInstance*) ip)->g_class))
#define _G_TYPE_IGI(ip, gt, ct)         ((ct*) g_type_interface_peek (((GTypeInstance*) ip)->g_class, gt))
#ifdef	__GNUC__
#  define _G_TYPE_CIT(ip, gt)             (G_GNUC_EXTENSION ({ \
  GTypeInstance *__inst = (GTypeInstance*) ip; GType __t = gt; gboolean __r; \
  if (__inst && __inst->g_class && __inst->g_class->g_type == __t) \
    __r = TRUE; \
  else \
    __r = g_type_check_instance_is_a (__inst, __t); \
  __r; \
}))
#  define _G_TYPE_CCT(cp, gt)             (G_GNUC_EXTENSION ({ \
  GTypeClass *__class = (GTypeClass*) cp; GType __t = gt; gboolean __r; \
  if (__class && __class->g_type == __t) \
    __r = TRUE; \
  else \
    __r = g_type_check_class_is_a (__class, __t); \
  __r; \
}))
#  define _G_TYPE_CVH(vl, gt)             (G_GNUC_EXTENSION ({ \
  GValue *__val = (GValue*) vl; GType __t = gt; gboolean __r; \
  if (__val && __val->g_type == __t) \
    __r = TRUE; \
  else \
    __r = g_type_check_value_holds (__val, __t); \
  __r; \
}))
#else  /* !__GNUC__ */
#  define _G_TYPE_CIT(ip, gt)             (g_type_check_instance_is_a ((GTypeInstance*) ip, gt))
#  define _G_TYPE_CCT(cp, gt)             (g_type_check_class_is_a ((GTypeClass*) cp, gt))
#  define _G_TYPE_CVH(vl, gt)             (g_type_check_value_holds ((GValue*) vl, gt))
#endif /* !__GNUC__ */


/* glib gtypes.h */
typedef char   gchar;
typedef short  gshort;
typedef long   glong;
typedef int    gint;
typedef gint   gboolean;

typedef unsigned char   guchar;
typedef unsigned short  gushort;
typedef unsigned long   gulong;
typedef unsigned int    guint;

typedef float   gfloat;
typedef double  gdouble;


typedef void* gpointer;
typedef const void *gconstpointer;

/* glib gobject/gtype.h */
typedef GTypeInstance* (*g_type_check_instance_cast_PROC)(GTypeInstance *instance, GType iface_type);
static g_type_check_instance_cast_PROC g_type_check_instance_cast;


/* glib gmem.h */
typedef void (*g_free_PROC)(gpointer mem);
static g_free_PROC g_free;

/* glib gslist.h */
typedef struct _GSList GSList;
struct _GSList
{
  gpointer data;
  GSList *next;
};

typedef void (*g_slist_free_PROC)(GSList* list);
static g_slist_free_PROC g_slist_free;

typedef guint (*g_slist_length_PROC)(GSList* list);
static g_slist_length_PROC g_slist_length;



/* From gtkwidget.h */
typedef struct GtkWidget         GtkWidget;

typedef void (*gtk_widget_destroy_PROC)(GtkWidget *widget);
static gtk_widget_destroy_PROC gtk_widget_destroy;


/* From gtkdialog.h */
typedef struct GtkDialog         GtkDialog;
typedef struct GtkDialogClass         GtkDialogClass;

/* Parameters for dialog construction */
typedef enum
{
  GTK_DIALOG_MODAL               = 1 << 0, /* call gtk_window_set_modal (win, TRUE) */
  GTK_DIALOG_DESTROY_WITH_PARENT = 1 << 1, /* call gtk_window_set_destroy_with_parent () */
  GTK_DIALOG_NO_SEPARATOR        = 1 << 2  /* no separator bar above buttons */
} GtkDialogFlags;

/* Convenience enum to use for response_id's.  Positive values are
 * totally user-interpreted. GTK will sometimes return
 * GTK_RESPONSE_NONE if no response_id is available.
 *
 *  Typical usage is:
 *     if (gtk_dialog_run(dialog) == GTK_RESPONSE_ACCEPT)
 *       blah();
 */
typedef enum
{
  /* GTK returns this if a response widget has no response_id,
   * or if the dialog gets programmatically hidden or destroyed.
   */
  GTK_RESPONSE_NONE = -1,

  /* GTK won't return these unless you pass them in
   * as the response for an action widget. They are
   * for your convenience.
   */
  GTK_RESPONSE_REJECT = -2,
  GTK_RESPONSE_ACCEPT = -3,

  /* If the dialog is deleted. */
  GTK_RESPONSE_DELETE_EVENT = -4,

  /* These are returned from GTK dialogs, and you can also use them
   * yourself if you like.
   */
  GTK_RESPONSE_OK     = -5,
  GTK_RESPONSE_CANCEL = -6,
  GTK_RESPONSE_CLOSE  = -7,
  GTK_RESPONSE_YES    = -8,
  GTK_RESPONSE_NO     = -9,
  GTK_RESPONSE_APPLY  = -10,
  GTK_RESPONSE_HELP   = -11
} GtkResponseType;

#define GTK_TYPE_DIALOG                  (gtk_dialog_get_type ())
#define GTK_DIALOG(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_DIALOG, GtkDialog))
#define GTK_DIALOG_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_DIALOG, GtkDialogClass))
#define GTK_IS_DIALOG(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_DIALOG))
#define GTK_IS_DIALOG_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_DIALOG))
#define GTK_DIALOG_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_DIALOG, GtkDialogClass))
typedef GType (*gtk_dialog_get_type_PROC)(void) G_GNUC_CONST;
static gtk_dialog_get_type_PROC gtk_dialog_get_type;


typedef gint (*gtk_dialog_run_PROC)(GtkDialog *dialog);
static gtk_dialog_run_PROC gtk_dialog_run;


/* From gtkfilefilter.h */
typedef struct GtkFileFilter     GtkFileFilter;


typedef GtkFileFilter* (*gtk_file_filter_new_PROC)(void);
static gtk_file_filter_new_PROC gtk_file_filter_new;

typedef void (*gtk_file_filter_set_name_PROC)(GtkFileFilter *filter, const gchar   *name);
static gtk_file_filter_set_name_PROC gtk_file_filter_set_name;


typedef void (*gtk_file_filter_add_pattern_PROC)(GtkFileFilter *filter, const gchar   *pattern);
static gtk_file_filter_add_pattern_PROC gtk_file_filter_add_pattern;


/* From gtkfilechooser.h */
typedef struct GtkFileChooser GtkFileChooser;
typedef struct GtkFileFilter GtkFileFilter;

/**
 * GtkFileChooserAction:
 * @GTK_FILE_CHOOSER_ACTION_OPEN: Indicates open mode.  The file chooser
 *  will only let the user pick an existing file.
 * @GTK_FILE_CHOOSER_ACTION_SAVE: Indicates save mode.  The file chooser
 *  will let the user pick an existing file, or type in a new
 *  filename.
 * @GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER: Indicates an Open mode for
 *  selecting folders.  The file chooser will let the user pick an
 *  existing folder.
 * @GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER: Indicates a mode for creating a
 *  new folder.  The file chooser will let the user name an existing or
 *  new folder.
 *
 * Describes whether a #GtkFileChooser is being used to open existing files
 * or to save to a possibly new file.
 */
typedef enum
{
  GTK_FILE_CHOOSER_ACTION_OPEN,
  GTK_FILE_CHOOSER_ACTION_SAVE,
  GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
  GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER
} GtkFileChooserAction;



#define GTK_TYPE_FILE_CHOOSER             (gtk_file_chooser_get_type ())
#define GTK_FILE_CHOOSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FILE_CHOOSER, GtkFileChooser))
#define GTK_IS_FILE_CHOOSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FILE_CHOOSER))
typedef GType (*gtk_file_chooser_get_type_PROC)(void) G_GNUC_CONST;
static gtk_file_chooser_get_type_PROC gtk_file_chooser_get_type;


typedef void (*gtk_file_chooser_add_filter_PROC)(GtkFileChooser *chooser, GtkFileFilter  *filter);
static gtk_file_chooser_add_filter_PROC gtk_file_chooser_add_filter;

typedef gboolean (*gtk_file_chooser_set_current_folder_PROC)(GtkFileChooser *chooser, const gchar    *filename);
static gtk_file_chooser_set_current_folder_PROC gtk_file_chooser_set_current_folder;


typedef gchar* (*gtk_file_chooser_get_filename_PROC)(GtkFileChooser *chooser);
static gtk_file_chooser_get_filename_PROC gtk_file_chooser_get_filename;

typedef GSList* (*gtk_file_chooser_get_filenames_PROC)(GtkFileChooser *chooser);
static gtk_file_chooser_get_filenames_PROC gtk_file_chooser_get_filenames;

typedef void (*gtk_file_chooser_set_select_multiple_PROC)(GtkFileChooser *chooser, gboolean select_multiple);
static gtk_file_chooser_set_select_multiple_PROC gtk_file_chooser_set_select_multiple;


typedef void (*gtk_file_chooser_set_do_overwrite_confirmation_PROC)(GtkFileChooser *chooser, gboolean do_overwrite_confirmation);
static gtk_file_chooser_set_do_overwrite_confirmation_PROC gtk_file_chooser_set_do_overwrite_confirmation;


typedef struct GtkWindow GtkWindow;

/* From gtkfilechooserdialog.h */
typedef GtkWidget* (*gtk_file_chooser_dialog_new_PROC)
(
 const gchar          *title,
 GtkWindow            *parent,
 GtkFileChooserAction  action,
 const gchar          *first_button_text,
 ...
) G_GNUC_NULL_TERMINATED;
static gtk_file_chooser_dialog_new_PROC gtk_file_chooser_dialog_new;


/* From gtkmain.h */
typedef gboolean (*gtk_events_pending_PROC)(void);
static gtk_events_pending_PROC gtk_events_pending;

typedef gboolean (*gtk_main_iteration_PROC)(void);
static gtk_main_iteration_PROC gtk_main_iteration;

typedef gboolean (*gtk_init_check_PROC)(int *argc, char ***argv);
static gtk_init_check_PROC gtk_init_check;

/* extra stuff */
#define TRUE 1
#define FALSE 0

#include <dlfcn.h>

static void NDFi_SetDLSymbols(void* glib_library, void* gtk_library)
{
	if(NULL != glib_library)
	{
		g_free = (g_free_PROC)dlsym(glib_library, "g_free");

		g_slist_free = (g_slist_free_PROC)dlsym(glib_library, "g_slist_free");
		g_slist_length = (g_slist_length_PROC)dlsym(glib_library, "g_slist_length");

		g_type_check_instance_cast = (g_type_check_instance_cast_PROC)dlsym(glib_library, "g_type_check_instance_cast");
	}
	if(NULL != gtk_library)
	{
		gtk_widget_destroy = (gtk_widget_destroy_PROC)dlsym(gtk_library, "gtk_widget_destroy");
		gtk_dialog_get_type = (gtk_dialog_get_type_PROC)dlsym(gtk_library, "gtk_dialog_get_type");
		gtk_dialog_run = (gtk_dialog_run_PROC)dlsym(gtk_library, "gtk_dialog_run");

		gtk_file_filter_new = (gtk_file_filter_new_PROC)dlsym(gtk_library, "gtk_file_filter_new");
		gtk_file_filter_set_name = (gtk_file_filter_set_name_PROC)dlsym(gtk_library, "gtk_file_filter_set_name");
		gtk_file_filter_add_pattern = (gtk_file_filter_add_pattern_PROC)dlsym(gtk_library, "gtk_file_filter_add_pattern");
		gtk_file_chooser_get_type = (gtk_file_chooser_get_type_PROC)dlsym(gtk_library, "gtk_file_chooser_get_type");
		gtk_file_chooser_add_filter = (gtk_file_chooser_add_filter_PROC)dlsym(gtk_library, "gtk_file_chooser_add_filter");
		gtk_file_chooser_set_current_folder = (gtk_file_chooser_set_current_folder_PROC)dlsym(gtk_library, "gtk_file_chooser_set_current_folder");
		gtk_file_chooser_get_filename = (gtk_file_chooser_get_filename_PROC)dlsym(gtk_library, "gtk_file_chooser_get_filename");
		gtk_file_chooser_get_filenames = (gtk_file_chooser_get_filenames_PROC)dlsym(gtk_library, "gtk_file_chooser_get_filenames");
		gtk_file_chooser_set_select_multiple = (gtk_file_chooser_set_select_multiple_PROC)dlsym(gtk_library, "gtk_file_chooser_set_select_multiple");
		gtk_file_chooser_set_do_overwrite_confirmation = (gtk_file_chooser_set_do_overwrite_confirmation_PROC)dlsym(gtk_library, "gtk_file_chooser_set_do_overwrite_confirmation");

		gtk_file_chooser_dialog_new = (gtk_file_chooser_dialog_new_PROC)dlsym(gtk_library, "gtk_file_chooser_dialog_new");

		gtk_events_pending = (gtk_events_pending_PROC)dlsym(gtk_library, "gtk_events_pending");
		gtk_main_iteration = (gtk_main_iteration_PROC)dlsym(gtk_library, "gtk_main_iteration");
		gtk_init_check = (gtk_init_check_PROC)dlsym(gtk_library, "gtk_init_check");
	}
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NFD_GTK_DLOPEN_H */

