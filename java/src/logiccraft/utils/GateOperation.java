package logiccraft.utils;

@FunctionalInterface
public interface GateOperation {

    boolean operate(boolean a, boolean b);
}