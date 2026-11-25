package logiccraft.collections;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Predicate;

/**
 * Custom Generic Collection for LogicCraft Circuit Components (CO3 & CO4 - Java Generics)
 *
 * Demonstrates:
 * - Type-safe generic storage: ComponentList<T>
 * - Bounds checking and safe element retrieval
 * - Removal, searching, and predicate-based filtering
 * - Iterable<T> support for enhanced for-loops
 *
 * @param <T> Component element type (e.g. LogicGate, Connection)
 */
public class ComponentList<T> implements Iterable<T> {

    private final List<T> components;

    public ComponentList() {
        this.components = new ArrayList<>();
    }

    public ComponentList(int initialCapacity) {
        this.components = new ArrayList<>(initialCapacity);
    }

    public void add(T component) {
        if (component == null) {
            throw new IllegalArgumentException("Cannot add null component to ComponentList.");
        }
        components.add(component);
    }

    public T get(int index) {
        if (index < 0 || index >= components.size()) {
            throw new IndexOutOfBoundsException("Index " + index + " out of bounds for ComponentList size " + components.size());
        }
        return components.get(index);
    }

    public T set(int index, T component) {
        if (component == null) {
            throw new IllegalArgumentException("Cannot set null component in ComponentList.");
        }
        if (index < 0 || index >= components.size()) {
            throw new IndexOutOfBoundsException("Index " + index + " out of bounds for ComponentList size " + components.size());
        }
        return components.set(index, component);
    }

    public T remove(int index) {
        if (index < 0 || index >= components.size()) {
            throw new IndexOutOfBoundsException("Index " + index + " out of bounds for ComponentList size " + components.size());
        }
        return components.remove(index);
    }

    public boolean remove(T component) {
        return components.remove(component);
    }

    public void clear() {
        components.clear();
    }

    public int size() {
        return components.size();
    }

    public boolean isEmpty() {
        return components.isEmpty();
    }

    public boolean contains(T component) {
        return components.contains(component);
    }

    public int indexOf(T component) {
        return components.indexOf(component);
    }

    /**
     * Filters components matching a generic predicate.
     * @param predicate filter criteria
     * @return new ComponentList<T> containing matching items
     */
    public ComponentList<T> filter(Predicate<? super T> predicate) {
        ComponentList<T> result = new ComponentList<>();
        for (T item : components) {
            if (predicate.test(item)) {
                result.add(item);
            }
        }
        return result;
    }

    public List<T> toList() {
        return new ArrayList<>(components);
    }

    @Override
    public Iterator<T> iterator() {
        return components.iterator();
    }

    @Override
    public String toString() {
        return "ComponentList[size=" + size() + ", items=" + components + "]";
    }
}
