
-jar ~/mpj-v0_44/lib/starter.jar -np 4

Unfortunately Java does not support MPI.


import mpi.*;

public class myScatterGather {
    public static void main(String[] args) throws Exception {
        MPI.Init(args);
        int rank = MPI.COMM_WORLD.Rank();
        int size = MPI.COMM_WORLD.Size();

        int unitsize = 4;
        int root = 0;
        int[] send_buffer = null;

        send_buffer = new int[unitsize * size];
        int[] recv_buffer = new int[unitsize];

        MPI.COMM_WORLD.Scatter(send_buffer, 0, unitsize, MPI.INT, recv_buffer, 0, unitsize, MPI.INT, root);

//        if (rank == 0) {
//            char[] message = "Hello, there".toCharArray();
//            MPI.COMM_WORLD.Send(message, 0, message.length, MPI.CHAR, 1, 99);
//        } else {
//            char[] message = new char[20];
//            MPI.COMM_WORLD.Recv(message, 0, 20, MPI.CHAR, 0, 99);
//            System.out.println("received:" + new String(message) + ":");
//        }

        for (int i = 0; i < unitsize; i++) {
            recv_buffer[i] = rank;
        }
        MPI.COMM_WORLD.Gather(recv_buffer, 0, unitsize, MPI.INT, send_buffer, 0, unitsize, MPI.INT, root);

        if(rank == root){
            for (int i = 0; i < (unitsize*size); i++) {
                System.out.println(send_buffer[i] + " ");
            }
        }
        MPI.Finalize();
    }
}

