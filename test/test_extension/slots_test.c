#include <Python.h>

typedef struct {
    PyObject_HEAD

    int n;
} slots_tester_object;

static PyObject *
slots_tester_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    slots_tester_object* obj;

    if (!_PyArg_NoKeywords("attrgetter()", kwds))
        return NULL;

    int n;
    if (!PyArg_ParseTuple(args, "n", &n))
        return NULL;

    /* create attrgetterobject structure */
    obj = PyObject_New(slots_tester_object, type);
    if (obj == NULL)
        return NULL;

    obj->n = n;

    return (PyObject *)obj;
}

static long slots_tester_seq_hash(slots_tester_object* obj) {
    printf("slots_tester_seq.__hash__\n");
    return obj->n ^ 1;
}

static PyObject* slots_tester_seq_richcmp(slots_tester_object* lhs, PyObject* rhs, int op) {
    printf("slots_tester_seq.richcmp(%d, %d)\n", lhs->n, op);

    Py_RETURN_TRUE;
    if (op % 2)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject *
slots_tester_seq_repr(slots_tester_object *obj)
{
    char buf[80];
    snprintf(buf, sizeof(buf), "<my custom repr: %d>", obj->n);
    return PyString_FromString(buf);
}

static PyObject *
slots_tester_seq_call(slots_tester_object *obj, PyObject *args, PyObject *kw)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    return PyInt_FromLong(obj->n);
}

static PyObject*
slots_tester_seq_item(slots_tester_object *obj, Py_ssize_t i)
{
    if (i < 0 || i >= 5) {
        PyErr_SetString(PyExc_IndexError, "tuple index out of range");
        return NULL;
    }
    return PyInt_FromLong(i + obj->n);
}

PyDoc_STRVAR(slots_tester_seq_doc, "slots_tester_seq doc");

static PySequenceMethods slots_tester_seq_as_sequence = {
    (lenfunc)0,
    (binaryfunc)0,           /* sq_concat */
    (ssizeargfunc)0,         /* sq_repeat */
    (ssizeargfunc)slots_tester_seq_item,               /* sq_item */
    (ssizessizeargfunc)0,         /* sq_slice */
    0,                                          /* sq_ass_item */
    0,                                          /* sq_ass_slice */
    (objobjproc)0,             /* sq_contains */
};


static PyTypeObject slots_tester_seq = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "slots_test.slots_tester_seq",            /* tp_name */
    sizeof(slots_tester_object),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    (reprfunc)slots_tester_seq_repr,        /* tp_repr */
    0,                                  /* tp_as_number */
    &slots_tester_seq_as_sequence,          /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    (hashfunc)slots_tester_seq_hash, /* tp_hash */
    (ternaryfunc)slots_tester_seq_call,     /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    slots_tester_seq_doc,                   /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    (richcmpfunc)slots_tester_seq_richcmp, /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    slots_tester_new,                   /* tp_new */
    0,                                  /* tp_free */
};

static Py_ssize_t slots_tester_map_length(slots_tester_object* obj) {
    return obj->n;
}

static PyObject* slots_tester_map_subscript(slots_tester_object* obj, PyObject* key) {
    int n2 = PyInt_AsLong(key);
    return PyInt_FromLong(n2 + obj->n);
}

static int slots_tester_map_ass_sub(slots_tester_object* obj, PyObject* key, PyObject* value) {
    int n2 = PyInt_AsLong(key);
    printf("Assigning to subscript for object with n=%d, key=%d, set/delete=%d\n", obj->n, n2, !!value);

    return 0;
}

static PyMappingMethods slots_tester_map_asmapping = {
    (lenfunc)slots_tester_map_length,              /*mp_length*/
    (binaryfunc)slots_tester_map_subscript,        /*mp_subscript*/
    (objobjargproc)slots_tester_map_ass_sub,       /*mp_ass_subscript*/
};

static PyTypeObject slots_tester_map= {
    PyVarObject_HEAD_INIT(NULL, 0)
    "slots_test.slots_tester_map",            /* tp_name */
    sizeof(slots_tester_object),          /* tp_basicsize */
    0,                                  /* tp_itemsize */
    /* methods */
    0,                                  /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,        /* tp_repr */
    0,                                  /* tp_as_number */
    0,          /* tp_as_sequence */
    &slots_tester_map_asmapping,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,     /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    0,                   /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    0,                                  /* tp_alloc */
    slots_tester_new,                   /* tp_new */
    0,                                  /* tp_free */
};

// Tests the correctness of the CAPI slots when the attributes get set in Python code:
static PyObject *
call_funcs(PyObject* _module, PyObject* args) {
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;

    printf("\n");

    PyTypeObject* cls = Py_TYPE(obj);
    printf("Received a %s object\n", cls->tp_name);

    if (cls->tp_repr) {
        PyObject* rtn = cls->tp_repr(obj);
        printf("tp_repr exists and returned: '%s'\n", PyString_AsString(rtn));
        Py_DECREF(rtn);
    }

    if (cls->tp_new) {
        PyObject* rtn = cls->tp_new(cls, PyTuple_New(0), PyDict_New());
        printf("tp_new exists and returned an object of type: '%s'\n", Py_TYPE(rtn)->tp_name);
        Py_DECREF(rtn);
    }

    if (cls->tp_call) {
        printf("tp_call exists\n");
    } else {
        printf("tp_call doesnt exist\n");
    }

    if (cls->tp_as_mapping) {
        printf("tp_as_mapping exists\n");
        PyMappingMethods* map = cls->tp_as_mapping;

        if (map->mp_subscript) {
            PyObject* rtn = map->mp_subscript(obj, PyInt_FromLong(1));
            printf("mp_subscript exists and returned\n");
            Py_DECREF(rtn);
        } else {
            printf("mp_subscript does not exist\n");
        }

        if (map->mp_length) {
            Py_ssize_t rtn = map->mp_length(obj);
            printf("mp_length exists and returned %ld\n", rtn);
        }
    } else {
        printf("tp_as_mapping doesnt exist\n");
    }

    if (cls->tp_as_sequence) {
        printf("tp_as_sequence exists\n");
        PySequenceMethods* seq = cls->tp_as_sequence;

        if (seq->sq_length) {
            Py_ssize_t rtn = seq->sq_length(obj);
            printf("sq_length exists and returned %ld\n", rtn);
        }

        if (seq->sq_item) {
            PyObject* rtn = cls->tp_as_sequence->sq_item(obj, 1);
            printf("sq_item exists and returned\n");
            Py_DECREF(rtn);
        }
    } else {
        printf("tp_as_sequence doesnt exist\n");
    }

    Py_DECREF(obj);

    Py_RETURN_NONE;
}

static PyMethodDef SlotsMethods[] = {
    {"call_funcs", call_funcs, METH_VARARGS, "Call slotted functions."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initslots_test(void)
{
    PyObject *m;

    m = Py_InitModule("slots_test", SlotsMethods);
    if (m == NULL)
        return;

    int res = PyType_Ready(&slots_tester_seq);
    if (res < 0)
        return;

    res = PyType_Ready(&slots_tester_map);
    if (res < 0)
        return;

    // Not sure if the result of PyInt_FromLong needs to be decref'd
    PyDict_SetItemString(slots_tester_seq.tp_dict, "set_through_tpdict", PyInt_FromLong(123));

    PyModule_AddObject(m, "SlotsTesterSeq", (PyObject *)&slots_tester_seq);
    PyModule_AddObject(m, "SlotsTesterMap", (PyObject *)&slots_tester_map);
}
