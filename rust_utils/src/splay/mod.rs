use std::cmp::Ordering::{self, Equal, Greater, Less};
use std::fmt::{self, Debug};
use std::mem::ManuallyDrop;
use std::ptr;

struct Node<T> {
    left: MaybeNode<T>,
    right: MaybeNode<T>,
    parent: ManuallyDrop<MaybeNode<T>>,
    data: T,
}

#[repr(transparent)]
struct MaybeNode<T> {
    inner: *mut Node<T>,
}

impl<T> Drop for MaybeNode<T> {
    fn drop(&mut self) {
        if let Some(inner) = self.as_mut() {
            unsafe { drop(Box::from_raw(inner)) }
        }
    }
}

pub trait UpdateNode: Debug {
    // update is called when the children change.
    fn update(&mut self, _left: Option<&Self>, _right: Option<&Self>) {}
    // push_down is called when node is accessed and the children will be visited.
    fn push_down(&mut self, _left: Option<&mut Self>, _right: Option<&mut Self>) {}
}

#[derive(Debug, Eq, PartialEq)]
enum ChildType {
    Left,
    Right,
}

impl<T> Node<T> {
    fn new(data: T, parent: *mut Node<T>) -> Self {
        Self {
            data,
            parent: ManuallyDrop::new(MaybeNode::new_weak(parent)),
            left: MaybeNode::empty(),
            right: MaybeNode::empty(),
        }
    }

    fn child_type_of(&self, child: *const Self) -> ChildType {
        debug_assert!(!child.is_null());
        debug_assert!(ptr::eq(self.left.inner, child) || ptr::eq(self.right.inner, child));
        match self.left.as_ref() {
            Some(left_child) if ptr::eq(left_child, child) => ChildType::Left,
            _ => ChildType::Right,
        }
    }

    fn child_type(&self) -> Option<ChildType> {
        self.parent
            .as_ref()
            .map(|parent| parent.child_type_of(self))
    }

    fn update_parent(&mut self, child_type: Option<ChildType>) {
        match child_type {
            None => {}
            Some(child_type) => {
                let self_ptr: *mut _ = self;
                self.parent.poke_child(child_type, self_ptr);
            }
        }
    }

    fn depth(&self) -> usize {
        std::cmp::max(self.left.depth(), self.right.depth()) + 1
    }

    fn far_left(&self) -> &Node<T> {
        let mut current = self;
        while let Some(left_child) = current.left.as_ref() {
            current = left_child;
        }
        current
    }

    fn successor(&self) -> Option<&Node<T>> {
        match self.right.as_ref() {
            None => {
                let mut current = self;
                while let Some(parent) = current.parent.as_ref() {
                    match parent.child_type_of(current) {
                        ChildType::Right => current = parent,
                        ChildType::Left => return Some(parent),
                    }
                }
                None
            }
            Some(right_child) => Some(right_child.far_left()),
        }
    }
}

impl<T> Node<T>
where
    T: UpdateNode,
{
    fn update(&mut self) {
        self.data.update(self.left.data(), self.right.data())
    }

    fn push_down(&mut self) {
        self.data
            .push_down(self.left.mut_data(), self.right.mut_data())
    }

    fn rotate_left(&mut self) -> &mut Self {
        assert!(self.right.as_ref().is_some());
        //   x
        // a   y
        //    b c
        let self_child_type = self.child_type();
        let y = unsafe {
            let y = self.right.inner.as_mut().unwrap_unchecked();

            y.parent.inner = self.parent.inner;
            self.parent.inner = y;

            y.left.poke_parent(self);
            self.right.inner = y.left.inner;
            y.left.inner = self;

            y.update_parent(self_child_type);

            y
        };
        self.update();
        y.update();
        y
        //    y
        //  x   c
        // a b
    }

    fn rotate_right(&mut self) -> &mut Self {
        assert!(self.left.as_ref().is_some());
        //   x
        // a   y
        //    b c
        let self_child_type = self.child_type();
        let y = unsafe {
            let y = self.left.inner.as_mut().unwrap_unchecked();

            y.parent.inner = self.parent.inner;
            self.parent.inner = y;

            y.right.poke_parent(self);
            self.left.inner = y.right.inner;
            y.right.inner = self;

            y.update_parent(self_child_type);

            y
        };
        self.update();
        y.update();
        y
        //    y
        //  x   c
        // a b
    }

    fn rotate_up(&mut self) -> &mut Self {
        match self.child_type() {
            None => panic!("Can't rotate the root up!"),
            Some(child_type) => self.rotate_up_with(child_type),
        }
    }

    fn rotate_up_with(&mut self, child_type: ChildType) -> &mut Self {
        debug_assert!(self.child_type().unwrap() == child_type);
        match child_type {
            ChildType::Left => self.parent.as_mut().unwrap().rotate_right(),
            ChildType::Right => self.parent.as_mut().unwrap().rotate_left(),
        }
    }

    fn splay(&mut self) -> &mut Self {
        while let Some(parent) = unsafe { self.parent.inner.as_mut() } {
            match unsafe { parent.parent.inner.as_mut() } {
                None => {
                    self.rotate_up();
                    assert!(self.parent.inner.is_null());
                    break;
                }
                Some(grandparent) => {
                    let self_child_type = parent.child_type_of(self);
                    let parent_child_type = grandparent.child_type_of(parent);
                    if self_child_type == parent_child_type {
                        // Zig-Zig
                        parent.rotate_up_with(parent_child_type);
                        self.rotate_up_with(self_child_type);
                    } else {
                        // Zig-Zag
                        self.rotate_up_with(self_child_type);
                        self.rotate_up_with(parent_child_type);
                    }
                }
            }
        }
        self
    }
}

impl<T> MaybeNode<T> {
    fn empty() -> Self {
        Self {
            inner: ptr::null_mut(),
        }
    }

    fn new_boxed(node: Node<T>) -> Self {
        Self {
            inner: Box::into_raw(Box::new(node)),
        }
    }

    fn new_weak(node: *mut Node<T>) -> Self {
        Self { inner: node }
    }

    fn as_ref(&self) -> Option<&Node<T>> {
        unsafe { self.inner.as_ref() }
    }

    fn as_mut(&mut self) -> Option<&mut Node<T>> {
        unsafe { self.inner.as_mut() }
    }

    fn data(&self) -> Option<&T> {
        self.as_ref().map(|node| &node.data)
    }

    fn mut_data(&mut self) -> Option<&mut T> {
        self.as_mut().map(|node| &mut node.data)
    }

    fn poke_child(&mut self, child_type: ChildType, new_val: *mut Node<T>) {
        if let Some(node) = self.as_mut() {
            match child_type {
                ChildType::Left => node.left.inner = new_val,
                ChildType::Right => node.right.inner = new_val,
            }
        }
    }

    fn poke_parent(&mut self, new_parent: *mut Node<T>) {
        if let Some(node) = self.as_mut() {
            node.parent.inner = new_parent;
        }
    }

    fn depth(&self) -> usize {
        self.as_ref().map_or(0, |node| node.depth())
    }

    fn far_left(&self) -> Option<&Node<T>> {
        match self.as_ref() {
            None => None,
            Some(node) => Some(node.far_left()),
        }
    }
}

struct AccessBottom<'a, T> {
    maybe_node: &'a mut MaybeNode<T>,
    parent: *mut Node<T>,
}

#[must_use]
enum AccessResult<'a, T> {
    Bottom(AccessBottom<'a, T>),
    Equal(&'a mut Node<T>),
}

impl<'a, T> AccessBottom<'a, T>
where
    T: UpdateNode,
{
    fn finalize<F>(self, f: F) -> *mut Node<T>
    where
        F: FnOnce(&mut MaybeNode<T>, *mut Node<T>),
    {
        f(self.maybe_node, self.parent);

        match self.maybe_node.as_mut() {
            Some(node) => node.splay(),
            None => match unsafe { self.parent.as_mut() } {
                Some(parent) => parent.splay(),
                None => ptr::null_mut(),
            },
        }
    }
}

impl<T> MaybeNode<T>
where
    T: UpdateNode,
{
    fn access<C>(mut current: &mut Self, mut comparator: C) -> AccessResult<'_, T>
    where
        C: FnMut(&T) -> Ordering,
    {
        let mut parent = ptr::null_mut();
        while let Some(node) = unsafe { current.inner.as_mut() } {
            match (comparator)(&node.data) {
                Less => {
                    node.push_down();
                    parent = node;
                    current = &mut node.left;
                }
                Greater => {
                    node.push_down();
                    parent = node;
                    current = &mut node.right;
                }
                Equal => {
                    node.splay();
                    return AccessResult::Equal(node);
                }
            }
        }
        AccessResult::Bottom(AccessBottom {
            maybe_node: current,
            parent,
        })
    }
}

pub struct Splay<T> {
    root: MaybeNode<T>,
}

pub struct Iter<'a, T> {
    node: Option<&'a Node<T>>,
}

impl<'a, T> Iterator for Iter<'a, T> {
    type Item = &'a T;
    fn next(&mut self) -> Option<Self::Item> {
        match &self.node {
            None => None,
            Some(node) => {
                let data = &node.data;
                self.node = node.successor();
                Some(data)
            }
        }
    }
}

impl<'a, T> IntoIterator for &'a Splay<T> {
    type IntoIter = Iter<'a, T>;
    type Item = &'a T;

    fn into_iter(self) -> Self::IntoIter {
        Iter {
            node: self.root.far_left(),
        }
    }
}

impl<T> Debug for Node<T>
where
    T: Debug,
{
    fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
        f.debug_struct("Node")
            .field("data", &self.data)
            .field("left", &self.left)
            .field("right", &self.right)
            .field("parent", &self.parent.data())
            .finish()
    }
}

impl<T> Debug for MaybeNode<T>
where
    T: Debug,
{
    fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
        Debug::fmt(&self.as_ref(), f)
    }
}

impl<T> Debug for Splay<T>
where
    T: Debug,
{
    fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
        f.debug_struct("Splay").field("root", &self.root).finish()
    }
}

impl<T> Splay<T>
where
    T: UpdateNode,
{
    pub fn new() -> Self {
        Self {
            root: MaybeNode::empty(),
        }
    }

    pub fn insert_with<F>(&mut self, data: T, mut cmp: F) -> Result<(), T>
    where
        F: FnMut(&T, &T) -> Ordering,
    {
        let access_result = MaybeNode::access(&mut self.root, |node_val| cmp(&data, node_val));
        match access_result {
            AccessResult::Equal(node) => {
                self.root.inner = node;
                Err(data)
            }
            AccessResult::Bottom(access_bottom) => {
                self.root.inner = access_bottom.finalize(|maybe_node, parent| {
                    *maybe_node = MaybeNode::new_boxed(Node::new(data, parent));
                });
                Ok(())
            }
        }
    }

    pub fn depth(&self) -> usize {
        self.root.depth()
    }
}

impl<T> Splay<T>
where
    T: Ord + UpdateNode,
{
    pub fn insert(&mut self, data: T) -> Result<(), T> {
        self.insert_with(data, T::cmp)
    }
}
